from SCons.Variables import BoolVariable

def can_build(env, platform):
    # 默认关闭，用户可选择启用
    return env.get("mcp_enabled", False)

def get_opts(platform):
    return [
        BoolVariable("mcp_enabled", "Enable MCP (Model Context Protocol) module", False)
    ]

def get_doc_classes():
    return [
        "MCPEditorPlugin",
        "MCPServer",
        "MCPCommandHandler"
    ]

def get_doc_path():
    return "doc_classes"
