# Godot MCP Module

This module provides Model Context Protocol (MCP) integration for Godot Engine, allowing AI assistants like Claude to interact with the editor through a WebSocket interface.

## Features

- **WebSocket Server**: Runs on port 9080 by default
- **AI Command Processing**: Handles various commands for scene manipulation, script editing, and more
- **Editor Integration**: Built-in editor plugin with control panel
- **Real-time Communication**: Bidirectional communication with AI assistants

## Architecture

The module is designed with minimal invasiveness to the core engine:

- **Core Layer**: MCP server and command handler
- **Editor Layer**: Editor plugin and UI panel
- **Command Layer**: Extensible command processing system

## Building

By default, the MCP module is **disabled** to maintain engine stability. To enable it:

1. Set the build option: `mcp_enabled=yes`
2. Rebuild the engine

```bash
scons mcp_enabled=yes platform=windows
```

## Usage

Once enabled, the MCP module will:

1. Automatically start when the editor launches
2. Display a control panel in the right dock
3. Listen for WebSocket connections on port 9080
4. Process AI commands in real-time

## Configuration

The module can be configured through the editor panel:

- **Port**: Change the WebSocket server port
- **Start/Stop**: Control the server manually
- **Logs**: View server activity and command processing

## Command System

The module supports various command types:

- Scene manipulation
- Script editing
- Node operations
- Project management
- Editor controls

## Dependencies

- Core Godot engine (no external dependencies)
- WebSocket support (built-in)
- TCP networking (built-in)

## License

This module follows the same MIT license as Godot Engine.
