/**************************************************************************/
/*  mcp_command_handler.cpp                                                */
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

#include "mcp_command_handler.h"
#include "mcp_server.h"
#include "core/string/print_string.h"

void MCPCommandHandler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_mcp_server", "server"), &MCPCommandHandler::set_mcp_server);
	ClassDB::bind_method(D_METHOD("get_mcp_server"), &MCPCommandHandler::get_mcp_server);
	ClassDB::bind_method(D_METHOD("handle_command", "client_id", "command"), &MCPCommandHandler::handle_command);
	ClassDB::bind_method(D_METHOD("add_command_processor", "processor"), &MCPCommandHandler::add_command_processor);
	ClassDB::bind_method(D_METHOD("remove_command_processor", "processor"), &MCPCommandHandler::remove_command_processor);
}

MCPCommandHandler::MCPCommandHandler() {
	print_line("MCP Command Handler initializing...");
}

MCPCommandHandler::~MCPCommandHandler() {
	command_processors.clear();
}

void MCPCommandHandler::set_mcp_server(MCPServer *p_server) {
	mcp_server = p_server;
	print_line("MCP Server reference set in command handler");
}

MCPServer *MCPCommandHandler::get_mcp_server() const {
	return mcp_server;
}

void MCPCommandHandler::handle_command(int client_id, const Dictionary &command) {
	String command_type = command.get("type", "");
	Dictionary params = command.get("params", Dictionary());
	String command_id = command.get("commandId", "");
	
	print_line("Processing command: " + command_type);
	
	// 尝试每个处理器直到一个处理命令
	for (Object *processor : command_processors) {
		if (processor && processor->has_method("process_command")) {
			// 调用处理器的process_command方法
			Variant result = processor->call("process_command", client_id, command_type, params, command_id);
			if (result.get_type() == Variant::BOOL && bool(result)) {
				return; // 命令已处理
			}
		}
	}
	
	// 如果没有处理器处理命令，发送错误
	_send_error(client_id, "Unknown command: " + command_type, command_id);
}

void MCPCommandHandler::add_command_processor(Object *processor) {
	if (processor && !command_processors.has(processor)) {
		command_processors.append(processor);
		print_line("Command processor added");
	}
}

void MCPCommandHandler::remove_command_processor(Object *processor) {
	command_processors.erase(processor);
	print_line("Command processor removed");
}

void MCPCommandHandler::_send_error(int client_id, const String &message, const String &command_id) {
	if (!mcp_server) {
		print_line("Error: MCP Server not available");
		return;
	}
	
	Dictionary response;
	response["status"] = "error";
	response["message"] = message;
	
	if (!command_id.is_empty()) {
		response["commandId"] = command_id;
	}
	
	mcp_server->send_response(client_id, response);
	print_line("Error: " + message);
}
