// https://www.rioki.org/2017/01/09/windows_stacktrace.html
#pragma once

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>
#include <sstream>

#pragma comment(lib, "dbghelp.lib")

#define FISX_ASSERT(COND) if ((COND) == false) {::FisxDbg::handle_assert(__FUNCTION__, #COND);}
#define FISX_FAILED(MSG) ::FisxDbg::fail(__FUNCTION__, MSG);
namespace FisxDbg
{
	struct StackFrame
	{
		DWORD64 address;
		std::string name;
		std::string module;
		unsigned int line;
		std::string file;
		unsigned int offset;
	};

	inline std::string basename(const std::string& file)
	{
		unsigned int i = file.find_last_of("\\/");
		if (i == std::string::npos)
		{
			return file;
		}
		else
		{
			return file.substr(i + 1);
		}
	}

	inline std::vector<StackFrame> stack_trace()
	{
		DWORD machine = IMAGE_FILE_MACHINE_AMD64;
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		if (SymInitialize(process, NULL, TRUE) == FALSE)
		{
			return {};
		}

		SymSetOptions(SYMOPT_LOAD_LINES);

		CONTEXT context = {};
		context.ContextFlags = CONTEXT_FULL;
		RtlCaptureContext(&context);

		STACKFRAME frame = {};
		frame.AddrPC.Offset = context.Rip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = context.Rbp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = context.Rsp;
		frame.AddrStack.Mode = AddrModeFlat;

		bool first = true;
		std::vector<StackFrame> frames;
		while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
		{
			StackFrame f = {};
			f.address = frame.AddrPC.Offset;

			DWORD64 moduleBase = 0;
			moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);
			char moduleBuff[MAX_PATH];
			if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduleBuff, MAX_PATH))
			{
				f.module = basename(moduleBuff);
			}
			else
			{
				f.module = "Unknown Module";
			}

			DWORD64 offset = 0;
			char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
			PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
			symbol->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 255;
			symbol->MaxNameLength = 254;
			if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol))
			{
				f.name = symbol->Name;
			}
			else
			{
				DWORD error = GetLastError();
				//DBG_TRACE(__FUNCTION__ ": Failed to resolve address 0x%X: %u\n", frame.AddrPC.Offset, error);
				f.name = "Unknown Function";
			}

			IMAGEHLP_LINE line;
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

			DWORD offset_ln = 0;
			if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line))
			{
				f.file = line.FileName;
				f.line = line.LineNumber;
				f.offset = offset_ln;
			}
			else
			{
				DWORD error = GetLastError();
				//DBG_TRACE(__FUNCTION__ ": Failed to resolve line for 0x%X: %u\n", frame.AddrPC.Offset, error);
				f.line = 0;
			}

			if (!first)
			{
				frames.push_back(f);
			}
			first = false;
		}

		SymCleanup(process);

		return frames;
	}

	inline void handle_assert(const char* func, const char* cond)
	{
		std::stringstream buff;
		buff << func << ": Assertion '" << cond << "' failed! \n";
		buff << "\n";

		std::vector<StackFrame> stack = stack_trace();
		buff << "Callstack: \n";
		for (unsigned int i = 0; i < stack.size(); i++)
		{
			buff << "0x" << std::hex << stack[i].address << ": " << stack[i].name << " line:" << std::dec << stack[i].line << " in " << stack[i].module << "\n";
		}
		MessageBoxA(NULL, buff.str().c_str(), "Assert Failed", MB_OK | MB_ICONSTOP);
		exit(-1);
	}

	inline void fail(const char* func, const char* msg)
	{
		std::stringstream buff;
		buff << func << ":  General Software Fault: '" << msg << "'! \n";
		buff << "\n";

		std::vector<StackFrame> stack = stack_trace();
		buff << "Callstack: \n";
		for (unsigned int i = 0; i < stack.size(); i++)
		{
			buff << "0x" << std::hex << stack[i].address << ": " << stack[i].name << " line:" << std::dec << stack[i].line << " in " << stack[i].module << "\n";
		}

		MessageBoxA(NULL, buff.str().c_str(), "General Software Fault", MB_OK | MB_ICONSTOP);
		exit(-1);
	}
}