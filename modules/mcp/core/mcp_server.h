/**************************************************************************/
/*  mcp_server.h                                                          */
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

#ifndef MCP_SERVER_H
#define MCP_SERVER_H

#include "core/object/object.h"
#include "core/io/tcp_server.h"
#include "core/io/stream_peer_tcp.h"
#include "core/io/websocket_peer.h"
#include "core/io/websocket_multiplayer_peer.h"
#include "core/variant/variant.h"
#include "core/string/string_name.h"

class MCPCommandHandler;

class MCPServer : public Object {
	GDCLASS(MCPServer, Object);

private:
	Ref<TCPServer> tcp_server;
	int port = 9080;
	int handshake_timeout = 3000; // ms
	bool debug_mode = true;
	bool log_detailed = true;
	
	MCPCommandHandler *command_handler = nullptr;
	
	// WebSocket客户端管理
	struct WebSocketClient {
		Ref<StreamPeerTCP> tcp;
		int id;
		Ref<WebSocketPeer> ws;
		int state = -1; // -1: handshaking, 0: connected, 1: error/closed
		uint64_t handshake_time;
		uint64_t last_poll_time;
		
		WebSocketClient(Ref<StreamPeerTCP> p_tcp, int p_id);
		bool upgrade_to_websocket();
	};
	
	HashMap<int, WebSocketClient> clients;
	int next_client_id = 1;
	
	// 日志缓冲区
	Vector<String> log_lines;
	
	// 信号
	void _client_connected(int id);
	void _client_disconnected(int id);
	void _command_received(int client_id, const Dictionary &command);
	
	// 内部方法
	void _process_clients();
	void _handle_websocket_message(int client_id, const String &message);
	void _send_response(int client_id, const Dictionary &response);
	void _log(const String &message);
	
protected:
	static void _bind_methods();
	
public:
	MCPServer();
	~MCPServer();
	
	// 服务器控制
	bool start_server();
	void stop_server();
	bool is_server_active() const;
	
	// 配置
	void set_port(int p_port);
	int get_port() const;
	void set_debug_mode(bool p_debug);
	bool get_debug_mode() const;
	
	// 命令处理
	void set_command_handler(MCPCommandHandler *p_handler);
	MCPCommandHandler *get_command_handler() const;
	
	// 日志管理
	void append_log(const String &message);
	String get_logs_text() const;
	void clear_logs();
	
	// 客户端管理
	int get_client_count() const;
	
	// 响应发送
	int send_response(int client_id, const Dictionary &response);
	
	// 处理循环
	void _process(double delta);
};

#endif // MCP_SERVER_H
