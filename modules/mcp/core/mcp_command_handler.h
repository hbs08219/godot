/**************************************************************************/
/*  mcp_command_handler.h                                                  */
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

#ifndef MCP_COMMAND_HANDLER_H
#define MCP_COMMAND_HANDLER_H

#include "core/object/object.h"
#include "core/variant/variant.h"

class MCPServer;

class MCPCommandHandler : public Object {
	GDCLASS(MCPCommandHandler, Object);

private:
	MCPServer *mcp_server = nullptr;
	
	// 命令处理器列表
	Vector<Object *> command_processors;
	
	// 内部方法
	void _initialize_command_processors();
	void _send_error(int client_id, const String &message, const String &command_id);

protected:
	static void _bind_methods();

public:
	MCPCommandHandler();
	~MCPCommandHandler();
	
	// 设置MCP服务器引用
	void set_mcp_server(MCPServer *p_server);
	MCPServer *get_mcp_server() const;
	
	// 处理命令
	void handle_command(int client_id, const Dictionary &command);
	
	// 添加命令处理器
	void add_command_processor(Object *processor);
	void remove_command_processor(Object *processor);
};

#endif // MCP_COMMAND_HANDLER_H
