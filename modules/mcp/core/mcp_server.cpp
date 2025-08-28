/**************************************************************************/
/*  mcp_server.cpp                                                        */
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

#include "mcp_server.h"
#include "mcp_command_handler.h"
#include "core/io/json.h"
#include "core/os/time.h"
#include "core/string/print_string.h"

// WebSocketClient 构造函数
MCPServer::WebSocketClient::WebSocketClient(Ref<StreamPeerTCP> p_tcp, int p_id) {
	tcp = p_tcp;
	id = p_id;
	handshake_time = Time::get_singleton()->get_ticks_msec();
	last_poll_time = handshake_time;
}

bool MCPServer::WebSocketClient::upgrade_to_websocket() {
	ws = Ref<WebSocketPeer>(memnew(WebSocketPeer));
	Error err = ws->accept_stream(tcp);
	return err == OK;
}

void MCPServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("start_server"), &MCPServer::start_server);
	ClassDB::bind_method(D_METHOD("stop_server"), &MCPServer::stop_server);
	ClassDB::bind_method(D_METHOD("is_server_active"), &MCPServer::is_server_active);
	ClassDB::bind_method(D_METHOD("set_port", "port"), &MCPServer::set_port);
	ClassDB::bind_method(D_METHOD("get_port"), &MCPServer::get_port);
	ClassDB::bind_method(D_METHOD("set_debug_mode", "debug"), &MCPServer::set_debug_mode);
	ClassDB::bind_method(D_METHOD("get_debug_mode"), &MCPServer::get_debug_mode);
	ClassDB::bind_method(D_METHOD("append_log", "message"), &MCPServer::append_log);
	ClassDB::bind_method(D_METHOD("get_logs_text"), &MCPServer::get_logs_text);
	ClassDB::bind_method(D_METHOD("clear_logs"), &MCPServer::clear_logs);
	ClassDB::bind_method(D_METHOD("get_client_count"), &MCPServer::get_client_count);
	ClassDB::bind_method(D_METHOD("send_response", "client_id", "response"), &MCPServer::send_response);
	
	ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_mode"), "set_debug_mode", "get_debug_mode");
}

MCPServer::MCPServer() {
	tcp_server = Ref<TCPServer>(memnew(TCPServer));
}

MCPServer::~MCPServer() {
	stop_server();
}

bool MCPServer::start_server() {
	if (tcp_server->is_listening()) {
		return true;
	}
	
	Error err = tcp_server->listen(port);
	if (err == OK) {
		_log("MCP Server started on port " + String::num_int64(port));
		return true;
	} else {
		_log("Failed to start MCP Server on port " + String::num_int64(port) + ", error: " + String::num_int64(err));
		return false;
	}
}

void MCPServer::stop_server() {
	if (tcp_server->is_listening()) {
		tcp_server->stop();
		clients.clear();
		_log("MCP Server stopped");
	}
}

bool MCPServer::is_server_active() const {
	return tcp_server->is_listening();
}

void MCPServer::set_port(int p_port) {
	port = p_port;
}

int MCPServer::get_port() const {
	return port;
}

void MCPServer::set_debug_mode(bool p_debug) {
	debug_mode = p_debug;
}

bool MCPServer::get_debug_mode() const {
	return debug_mode;
}

void MCPServer::set_command_handler(MCPCommandHandler *p_handler) {
	command_handler = p_handler;
}

MCPCommandHandler *MCPServer::get_command_handler() const {
	return command_handler;
}

void MCPServer::append_log(const String &message) {
	log_lines.append(message);
	_log(message);
}

String MCPServer::get_logs_text() const {
	return String("\n").join(log_lines);
}

void MCPServer::clear_logs() {
	log_lines.clear();
}

int MCPServer::get_client_count() const {
	return clients.size();
}

int MCPServer::send_response(int client_id, const Dictionary &response) {
	if (!clients.has(client_id)) {
		_log("Error: Client " + String::num_int64(client_id) + " not found");
		return ERR_DOES_NOT_EXIST;
	}
	
	WebSocketClient &client = clients[client_id];
	if (client.ws.is_null() || client.ws->get_ready_state() != WebSocketPeer::STATE_OPEN) {
		_log("Error: Client " + String::num_int64(client_id) + " connection not open");
		return ERR_UNAVAILABLE;
	}
	
	// 将响应转换为JSON
	JSON json;
	String json_text = json.stringify(response);
	
	_log("Sending response to client " + String::num_int64(client_id) + ": " + json_text);
	
	Error result = client.ws->send_text(json_text);
	if (result != OK) {
		_log("Error sending response to client " + String::num_int64(client_id) + ": " + String::num_int64(result));
	}
	
	return result;
}

void MCPServer::_process(double delta) {
	if (!tcp_server->is_listening()) {
		return;
	}
	
	_process_clients();
}

void MCPServer::_process_clients() {
	// 处理新连接
	if (tcp_server->is_connection_available()) {
		Ref<StreamPeerTCP> tcp = tcp_server->take_connection();
		int id = next_client_id++;
		
		WebSocketClient client(tcp, id);
		clients[id] = client;
		
		_log("New TCP connection from client " + String::num_int64(id));
		
		// 尝试立即升级到WebSocket
		if (client.upgrade_to_websocket()) {
			_log("WebSocket handshake started for client " + String::num_int64(id));
		} else {
			_log("Failed to start WebSocket handshake for client " + String::num_int64(id));
			clients.erase(id);
		}
	}
	
	// 更新现有客户端
	uint64_t current_time = Time::get_singleton()->get_ticks_msec();
	Vector<int> ids_to_remove;
	
	for (KeyValue<int, WebSocketClient> &kv : clients) {
		int id = kv.key;
		WebSocketClient &client = kv.value;
		client.last_poll_time = current_time;
		
		// 根据客户端状态处理
		if (client.state == -1) { // 握手中
			if (client.ws.is_valid()) {
				client.ws->poll();
				
				WebSocketPeer::State ws_state = client.ws->get_ready_state();
				if (debug_mode) {
					_log("Client " + String::num_int64(id) + " WebSocket state: " + String::num_int64(ws_state));
				}
				
				if (ws_state == WebSocketPeer::STATE_OPEN) {
					_log("WebSocket handshake completed for client " + String::num_int64(id));
					client.state = 0;
					_client_connected(id);
					
					// 发送欢迎消息
					Dictionary welcome_msg;
					welcome_msg["type"] = "welcome";
					welcome_msg["message"] = "Welcome to Godot MCP WebSocket Server";
					_send_response(id, welcome_msg);
					
				} else if (ws_state != WebSocketPeer::STATE_CONNECTING) {
					_log("WebSocket handshake failed for client " + String::num_int64(id) + ", state: " + String::num_int64(ws_state));
					ids_to_remove.append(id);
				} else if (current_time - client.handshake_time > handshake_timeout) {
					_log("WebSocket handshake timed out for client " + String::num_int64(id));
					ids_to_remove.append(id);
				}
			} else {
				// 如果TCP仍然连接，尝试升级
				if (client.tcp->get_status() == StreamPeerTCP::STATUS_CONNECTED) {
					if (client.upgrade_to_websocket()) {
						_log("WebSocket handshake started for client " + String::num_int64(id));
					} else {
						_log("Failed to start WebSocket handshake for client " + String::num_int64(id));
						ids_to_remove.append(id);
					}
				} else {
					_log("TCP disconnected during handshake for client " + String::num_int64(id));
					ids_to_remove.append(id);
				}
			}
		} else if (client.state == 0) { // 已连接
			client.ws->poll();
			
			WebSocketPeer::State ws_state = client.ws->get_ready_state();
			if (ws_state != WebSocketPeer::STATE_OPEN) {
				_log("WebSocket connection closed for client " + String::num_int64(id) + ", state: " + String::num_int64(ws_state));
				_client_disconnected(id);
				ids_to_remove.append(id);
				continue;
			}
			
			// 处理消息
			while (client.ws->get_available_packet_count() > 0) {
				PackedByteArray packet = client.ws->get_packet();
				String text = packet.get_string_from_utf8();
				
				_log("Received from client " + String::num_int64(id) + ": " + text);
				_handle_websocket_message(id, text);
			}
		}
	}
	
	// 移除需要删除的客户端
	for (int id : ids_to_remove) {
		clients.erase(id);
	}
}

void MCPServer::_handle_websocket_message(int client_id, const String &message) {
	// 解析JSON
	JSON json;
	Error parse_result = json.parse(message);
	
	if (parse_result == OK) {
		Variant data = json.get_data();
		
		if (data.get_type() == Variant::DICTIONARY) {
			Dictionary dict = data;
			
			// 处理JSON-RPC协议
			if (dict.has("jsonrpc") && dict["jsonrpc"] == "2.0") {
				if (dict.has("method") && dict["method"] == "ping") {
					// 处理ping方法
					Dictionary response;
					response["jsonrpc"] = "2.0";
					response["id"] = dict.get("id", Variant());
					response["result"] = Variant();
					_send_response(client_id, response);
				} else if (dict.has("method")) {
					// 处理其他MCP命令
					Dictionary response;
					response["jsonrpc"] = "2.0";
					response["id"] = dict.get("id", Variant());
					response["result"] = Dictionary();
					response["result"]["status"] = "success";
					response["result"]["message"] = "Command processed";
					_send_response(client_id, response);
				}
			} else if (dict.has("type")) {
				// 处理传统命令格式
				_command_received(client_id, dict);
			}
		}
	} else {
		_log("Failed to parse JSON from client " + String::num_int64(client_id) + ": " + json.get_error_message());
	}
}

void MCPServer::_client_connected(int id) {
	_log("Client " + String::num_int64(id) + " connected");
}

void MCPServer::_client_disconnected(int id) {
	_log("Client " + String::num_int64(id) + " disconnected");
}

void MCPServer::_command_received(int client_id, const Dictionary &command) {
	_log("Command received from client " + String::num_int64(client_id) + ": " + String(command.get("type", "unknown")));
	
	// 如果有命令处理器，转发命令
	if (command_handler) {
		command_handler->handle_command(client_id, command);
	}
}

void MCPServer::_log(const String &message) {
	if (log_detailed) {
		print_line("[MCP Server] " + message);
	}
}
