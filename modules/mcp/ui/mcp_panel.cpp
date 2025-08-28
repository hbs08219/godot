/**************************************************************************/
/*  mcp_panel.cpp                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "mcp_panel.h"
#include "mcp_server.h"
#include "core/string/print_string.h"

void MCPPanel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_mcp_server", "server"), &MCPPanel::set_mcp_server);
	ClassDB::bind_method(D_METHOD("get_mcp_server"), &MCPPanel::get_mcp_server);
	ClassDB::bind_method(D_METHOD("get_log_text"), &MCPPanel::get_log_text);
	ClassDB::bind_method(D_METHOD("add_log_message", "message"), &MCPPanel::add_log_message);
	ClassDB::bind_method(D_METHOD("clear_logs"), &MCPPanel::clear_logs);
	
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mcp_server", PROPERTY_HINT_RESOURCE_TYPE, "MCPServer"), "set_mcp_server", "get_mcp_server");
}

MCPPanel::MCPPanel() {
	print_line("MCP Panel created");
	_create_ui();
}

MCPPanel::~MCPPanel() {
	print_line("MCP Panel destroyed");
}

void MCPPanel::_create_ui() {
	// 创建主容器
	main_container = memnew(VBoxContainer);
	add_child(main_container);
	
	// 状态容器
	HBoxContainer *status_container = memnew(HBoxContainer);
	main_container->add_child(status_container);
	
	Label *status_title = memnew(Label);
	status_title->set_text("Status:");
	status_container->add_child(status_title);
	
	status_label = memnew(Label);
	status_label->set_text("Not initialized");
	status_container->add_child(status_label);
	
	// 端口容器
	HBoxContainer *port_container = memnew(HBoxContainer);
	main_container->add_child(port_container);
	
	Label *port_title = memnew(Label);
	port_title->set_text("Port:");
	port_container->add_child(port_title);
	
	port_input = memnew(SpinBox);
	port_input->set_min(1024);
	port_input->set_max(65535);
	port_input->set_value(9080);
	port_input->set_step(1);
	port_container->add_child(port_input);
	
	// 按钮容器
	HBoxContainer *buttons_container = memnew(HBoxContainer);
	main_container->add_child(buttons_container);
	
	start_button = memnew(Button);
	start_button->set_text("Start Server");
	buttons_container->add_child(start_button);
	
	stop_button = memnew(Button);
	stop_button->set_text("Stop Server");
	buttons_container->add_child(stop_button);
	
	// 连接计数容器
	HBoxContainer *connections_container = memnew(HBoxContainer);
	main_container->add_child(connections_container);
	
	Label *connections_title = memnew(Label);
	connections_title->set_text("Connections:");
	connections_container->add_child(connections_title);
	
	connection_count_label = memnew(Label);
	connection_count_label->set_text("0");
	connections_container->add_child(connection_count_label);
	
	// 日志容器
	HBoxContainer *log_container = memnew(HBoxContainer);
	main_container->add_child(log_container);
	
	Label *log_title = memnew(Label);
	log_title->set_text("Server Logs:");
	log_container->add_child(log_title);
	
	Button *clear_logs_button = memnew(Button);
	clear_logs_button->set_text("Clear");
	clear_logs_button->connect("pressed", Callable(this, "clear_logs"));
	log_container->add_child(clear_logs_button);
	
	// 日志文本区域
	log_text = memnew(TextEdit);
	log_text->set_custom_minimum_size(Vector2(300, 200));
	log_text->set_editable(false);
	log_text->set_wrap_mode(TextEdit::LINE_WRAPPING_BOUNDARY);
	main_container->add_child(log_text);
	
	// 连接信号
	start_button->connect("pressed", Callable(this, "_on_start_button_pressed"));
	stop_button->connect("pressed", Callable(this, "_on_stop_button_pressed"));
	port_input->connect("value_changed", Callable(this, "_on_port_changed"));
	
	// 初始UI状态
	_update_ui();
	
	print_line("MCP Panel UI created");
}

void MCPPanel::_update_ui() {
	if (!mcp_server.is_valid()) {
		status_label->set_text("Server: Not initialized");
		start_button->set_disabled(true);
		stop_button->set_disabled(true);
		port_input->set_editable(true);
		connection_count_label->set_text("0");
		return;
	}
	
	bool is_active = mcp_server->is_server_active();
	status_label->set_text("Server: " + (is_active ? "Running" : "Stopped"));
	start_button->set_disabled(is_active);
	stop_button->set_disabled(!is_active);
	port_input->set_editable(!is_active);
	
	if (is_active) {
		connection_count_label->set_text(String::num_int64(mcp_server->get_client_count()));
	} else {
		connection_count_label->set_text("0");
	}
}

void MCPPanel::_log_message(const String &message) {
	if (log_text) {
		log_text->append_text(message + "\n");
		// 滚动到底部
		log_text->set_caret_line(log_text->get_line_count() - 1);
	}
}

void MCPPanel::_on_start_button_pressed() {
	if (mcp_server.is_valid()) {
		if (mcp_server->start_server()) {
			_log_message("Server started on port " + String::num_int64(mcp_server->get_port()));
		} else {
			_log_message("Failed to start server");
		}
		_update_ui();
	}
}

void MCPPanel::_on_stop_button_pressed() {
	if (mcp_server.is_valid()) {
		mcp_server->stop_server();
		_log_message("Server stopped");
		_update_ui();
	}
}

void MCPPanel::_on_port_changed(double new_port) {
	if (mcp_server.is_valid()) {
		mcp_server->set_port(int(new_port));
		_log_message("Port changed to " + String::num_int64(int(new_port)));
	}
}

void MCPPanel::set_mcp_server(Ref<MCPServer> p_server) {
	mcp_server = p_server;
	if (mcp_server.is_valid()) {
		port_input->set_value(mcp_server->get_port());
		_log_message("MCP Server reference set");
	}
	_update_ui();
}

Ref<MCPServer> MCPPanel::get_mcp_server() const {
	return mcp_server;
}

String MCPPanel::get_log_text() const {
	if (log_text) {
		return log_text->get_text();
	}
	return "";
}

void MCPPanel::add_log_message(const String &message) {
	_log_message(message);
}

void MCPPanel::clear_logs() {
	if (log_text) {
		log_text->clear();
	}
	_log_message("Logs cleared");
}
