/**************************************************************************/
/*  mcp_editor_plugin.h                                                   */
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

#ifndef MCP_EDITOR_PLUGIN_H
#define MCP_EDITOR_PLUGIN_H

#include "editor/plugins/editor_plugin.h"
#include "core/object/ref_counted.h"

class MCPServer;
class MCPCommandHandler;
class MCPPanel;

class MCPEditorPlugin : public EditorPlugin {
	GDCLASS(MCPEditorPlugin, EditorPlugin);

private:
	Ref<MCPServer> mcp_server;
	Ref<MCPCommandHandler> command_handler;
	Ref<MCPPanel> mcp_panel;
	
	// 内部方法
	void _initialize_mcp();
	void _cleanup_mcp();

protected:
	static void _bind_methods();

public:
	MCPEditorPlugin();
	~MCPEditorPlugin();
	
	// EditorPlugin接口
	void _enter_tree() override;
	void _exit_tree() override;
	
	// 获取插件信息
	String get_plugin_name() const override;
};

#endif // MCP_EDITOR_PLUGIN_H
