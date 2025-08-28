/**************************************************************************/
/*  mcp_editor_plugin.cpp                                                 */
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

#include "mcp_editor_plugin.h"
#include "mcp_server.h"
#include "mcp_command_handler.h"
#include "mcp_panel.h"
#include "core/string/print_string.h"

void MCPEditorPlugin::_bind_methods() {
	// 这里可以绑定额外的方法如果需要
}

MCPEditorPlugin::MCPEditorPlugin() {
	print_line("MCP Editor Plugin created");
}

MCPEditorPlugin::~MCPEditorPlugin() {
	_cleanup_mcp();
}

void MCPEditorPlugin::_enter_tree() {
	print_line("MCP Editor Plugin entering tree");
	
	// 延迟初始化MCP，确保编辑器完全加载
	call_deferred("_initialize_mcp");
}

void MCPEditorPlugin::_exit_tree() {
	print_line("MCP Editor Plugin exiting tree");
	_cleanup_mcp();
}

String MCPEditorPlugin::get_plugin_name() const {
	return "Godot MCP";
}

void MCPEditorPlugin::_initialize_mcp() {
	print_line("Initializing MCP system...");
	
	// 创建MCP服务器
	mcp_server = Ref<MCPServer>(memnew(MCPServer));
	
	// 创建命令处理器
	command_handler = Ref<MCPCommandHandler>(memnew(MCPCommandHandler));
	
	// 设置相互引用
	command_handler->set_mcp_server(mcp_server.ptr());
	mcp_server->set_command_handler(command_handler.ptr());
	
	// 创建MCP面板
	mcp_panel = Ref<MCPPanel>(memnew(MCPPanel));
	
	// 将面板添加到编辑器停靠区域
	add_control_to_dock(DOCK_SLOT_RIGHT_UL, mcp_panel);
	
	// 启动MCP服务器
	if (mcp_server->start_server()) {
		print_line("MCP Server started successfully on port " + String::num_int64(mcp_server->get_port()));
	} else {
		print_line("Failed to start MCP Server");
	}
	
	print_line("MCP system initialized successfully");
}

void MCPEditorPlugin::_cleanup_mcp() {
	print_line("Cleaning up MCP system...");
	
	// 停止MCP服务器
	if (mcp_server.is_valid()) {
		mcp_server->stop_server();
	}
	
	// 移除面板
	if (mcp_panel.is_valid()) {
		remove_control_from_docks(mcp_panel);
	}
	
	// 清理引用
	mcp_panel.unref();
	command_handler.unref();
	mcp_server.unref();
	
	print_line("MCP system cleaned up");
}
