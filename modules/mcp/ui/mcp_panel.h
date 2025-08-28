/**************************************************************************/
/*  mcp_panel.h                                                           */
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

#ifndef MCP_PANEL_H
#define MCP_PANEL_H

#include "scene/gui/control.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/v_box_container.h"
#include "scene/gui/h_box_container.h"
#include "core/object/ref_counted.h"

class MCPServer;

class MCPPanel : public Control {
	GDCLASS(MCPPanel, Control);

private:
	Ref<MCPServer> mcp_server;
	
	// UI控件
	VBoxContainer *main_container = nullptr;
	Label *status_label = nullptr;
	SpinBox *port_input = nullptr;
	Button *start_button = nullptr;
	Button *stop_button = nullptr;
	Label *connection_count_label = nullptr;
	TextEdit *log_text = nullptr;
	
	// 内部方法
	void _create_ui();
	void _update_ui();
	void _log_message(const String &message);
	
	// 信号处理
	void _on_start_button_pressed();
	void _on_stop_button_pressed();
	void _on_port_changed(double new_port);

protected:
	static void _bind_methods();

public:
	MCPPanel();
	~MCPPanel();
	
	// 设置MCP服务器引用
	void set_mcp_server(Ref<MCPServer> p_server);
	Ref<MCPServer> get_mcp_server() const;
	
	// 获取日志文本
	String get_log_text() const;
	
	// 添加日志消息
	void add_log_message(const String &message);
	
	// 清除日志
	void clear_logs();
};

#endif // MCP_PANEL_H
