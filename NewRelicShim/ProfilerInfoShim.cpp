#include "ProfilerInfoShim.h"

ProfilerInfoShim::ProfilerInfoShim(ICorProfilerInfo3 *wrappedInfo, FILE *stream) :
	m_wrappedInfo(wrappedInfo),
	m_stream(stream)
{
	m_wrappedInfo->AddRef();
}

ProfilerInfoShim::~ProfilerInfoShim()
{
	m_wrappedInfo->Release();
}

ULONG ProfilerInfoShim::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

ULONG ProfilerInfoShim::Release()
{
	ULONG refCount = InterlockedDecrement(&m_refCount);
	if (refCount == 0)
		delete this;

	return refCount;
}

HRESULT ProfilerInfoShim::QueryInterface(REFIID riid, void **ppInterface)
{
	if (riid == IID_IUnknown)
        *ppInterface = static_cast<IUnknown *>(this); 
    else if (riid == IID_ICorProfilerInfo)
        *ppInterface = static_cast<ICorProfilerInfo *>(this);
    else if (riid == IID_ICorProfilerInfo2)
        *ppInterface = static_cast<ICorProfilerInfo2 *>(this);
	else if (riid == IID_ICorProfilerInfo3)
		*ppInterface = static_cast<ICorProfilerInfo3 *>(this);
    else
        *ppInterface = NULL;

	if (ppInterface == NULL)
		return E_NOINTERFACE;

    reinterpret_cast<IUnknown *>(*ppInterface)->AddRef();

    return S_OK;
}

HRESULT ProfilerInfoShim::GetClassFromObject(ObjectID objectId, ClassID *pClassId)
{
	fprintf(m_stream, "Call GetClassFromObject\n");
	fflush(m_stream);
	return m_wrappedInfo->GetClassFromObject(objectId, pClassId);
}

HRESULT ProfilerInfoShim::GetClassFromToken(ModuleID moduleId, mdTypeDef typeDef, ClassID *pClassId)
{
	fprintf(m_stream, "Call GetClassFromToken\n");
	fflush(m_stream);
	return m_wrappedInfo->GetClassFromToken(moduleId, typeDef, pClassId);
}

HRESULT ProfilerInfoShim::GetCodeInfo(FunctionID functionId, LPCBYTE *pStart, ULONG *pcSize)
{
	fprintf(m_stream, "Call GetCodeInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetCodeInfo(functionId, pStart, pcSize);
}

HRESULT ProfilerInfoShim::GetEventMask(DWORD *pdwEvents)
{
	return m_wrappedInfo->GetEventMask(pdwEvents);
}

HRESULT ProfilerInfoShim::GetFunctionFromIP(LPCBYTE ip, FunctionID *pFunctionId)
{
	fprintf(m_stream, "Call GetFunctionFromIP\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionFromIP(ip, pFunctionId);
}

HRESULT ProfilerInfoShim::GetFunctionFromToken(ModuleID moduleId, mdToken token, FunctionID *pFunctionId)
{
	fprintf(m_stream, "Call GetFunctionFromToken\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionFromToken(moduleId, token, pFunctionId);
}

HRESULT ProfilerInfoShim::GetHandleFromThread(ThreadID threadId, HANDLE *phThread)
{
	fprintf(m_stream, "Call GetHandleFromThread\n");
	fflush(m_stream);
	return m_wrappedInfo->GetHandleFromThread(threadId, phThread);
}

HRESULT ProfilerInfoShim::GetObjectSize(ObjectID objectId, ULONG *pcSize)
{
	fprintf(m_stream, "Call GetObjectSize\n");
	fflush(m_stream);
	return m_wrappedInfo->GetObjectSize(objectId, pcSize);
}

HRESULT ProfilerInfoShim::IsArrayClass(ClassID classId, CorElementType *pBaseElemType, ClassID *pBaseClassId, ULONG *pcRank)
{
	fprintf(m_stream, "Call IsArrayClass\n");
	fflush(m_stream);
	return m_wrappedInfo->IsArrayClass(classId, pBaseElemType, pBaseClassId, pcRank);
}

HRESULT ProfilerInfoShim::GetThreadInfo(ThreadID threadId, DWORD *pdwWin32ThreadId)
{
	fprintf(m_stream, "Call GetThreadInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetThreadInfo(threadId, pdwWin32ThreadId);
}

HRESULT ProfilerInfoShim::GetCurrentThreadID(ThreadID *pThreadId)
{
	fprintf(m_stream, "Call GetCurrentThreadID\n");
	fflush(m_stream);
	return m_wrappedInfo->GetCurrentThreadID(pThreadId);
}

HRESULT ProfilerInfoShim::GetClassIDInfo(ClassID classId, ModuleID *pModuleId, mdTypeDef *pTypeDefToken)
{
	fprintf(m_stream, "Call GetClassIDInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetClassIDInfo(classId, pModuleId, pTypeDefToken);
}

HRESULT ProfilerInfoShim::GetFunctionInfo(FunctionID functionId, ClassID *pClassId, ModuleID *pModuleId, mdToken *pToken)
{
	ModuleID moduleId;
	HRESULT result = m_wrappedInfo->GetFunctionInfo(functionId, pClassId, &moduleId, pToken);
	if (pModuleId != NULL)
		*pModuleId = moduleId;
	if (result != S_OK)
		return result;

	return result;
}

HRESULT ProfilerInfoShim::SetEventMask(DWORD dwEvents)
{
	fprintf(m_stream, "Call SetEventMask\n");
	fflush(m_stream);
	return m_wrappedInfo->SetEventMask(dwEvents);
}

HRESULT ProfilerInfoShim::SetEnterLeaveFunctionHooks(FunctionEnter *pFuncEnter, FunctionLeave *pFuncLeave, FunctionTailcall *pFuncTailcall)
{
	fprintf(m_stream, "Call SetEnterLeaveFunctionHooks\n");
	fflush(m_stream);
	return m_wrappedInfo->SetEnterLeaveFunctionHooks(pFuncEnter, pFuncLeave, pFuncTailcall);
}

HRESULT ProfilerInfoShim::SetFunctionIDMapper(FunctionIDMapper *pFunc)
{
	fprintf(m_stream, "Call SetFunctionIDMapper\n");
	fflush(m_stream);
	return m_wrappedInfo->SetFunctionIDMapper(pFunc);
}

HRESULT ProfilerInfoShim::GetTokenAndMetaDataFromFunction(FunctionID functionId, REFIID riid, IUnknown **ppImport, mdToken *pToken)
{
	HRESULT result = m_wrappedInfo->GetTokenAndMetaDataFromFunction(functionId, riid, ppImport, pToken);
	if (result != S_OK)
		return result;

	return result;
}

HRESULT ProfilerInfoShim::GetModuleInfo(ModuleID moduleId, LPCBYTE *ppBaseLoadAddress, ULONG cchName, ULONG *pcchName, WCHAR szName[  ], AssemblyID *pAssemblyId)
{
	HRESULT result = m_wrappedInfo->GetModuleInfo(moduleId, ppBaseLoadAddress, cchName, pcchName, szName, pAssemblyId);
	if (result != S_OK)
		return result;

	return result;
}

HRESULT ProfilerInfoShim::GetModuleMetaData(ModuleID moduleId, DWORD dwOpenFlags, REFIID riid, IUnknown **ppOut)
{
	fprintf(m_stream, "Call GetModuleMetaData module 0x%x open flags 0x%x, interface %x-%x-%x-%x%x%x%x%x%x%x%x\n", moduleId, dwOpenFlags, riid.Data1, riid.Data2, riid.Data3, riid.Data4[0], riid.Data4[1], riid.Data4[2], riid.Data4[3], riid.Data4[4], riid.Data4[5], riid.Data4[6], riid.Data4[7]);
	fflush(m_stream);
	return m_wrappedInfo->GetModuleMetaData(moduleId, dwOpenFlags, riid, ppOut);
}

void PrintByteInstruction(FILE *m_stream, BYTE *codeBytes, UINT &offset, const char *opcode)
{
	offset++;
	fprintf(m_stream, "%s\t0x%02x", opcode, codeBytes[offset]);
}

void PrintInt16Instruction(FILE *m_stream, BYTE *codeBytes, UINT &offset, const char *opcode)
{
	UINT16 value = *((UINT16 *) &codeBytes[offset + 1]);
	offset += 2;
	fprintf(m_stream, "%s\t0x%04x", opcode, value);
}

void PrintInt32Instruction(FILE *m_stream, BYTE *codeBytes, UINT &offset, const char *opcode)
{
	UINT32 value = *((UINT32 *) &codeBytes[offset + 1]);
	offset += 4;
	fprintf(m_stream, "%s\t0x%08x", opcode, value);
}

void PrintInt64Instruction(FILE *m_stream, BYTE *codeBytes, UINT &offset, const char *opcode)
{
	UINT64 value = *((UINT64 *) &codeBytes[offset + 1]);
	offset += 8;
	fprintf(m_stream, "%s\t0x%16x", opcode, value);
}

void PrintLongInstruction(FILE *m_stream, BYTE *codeBytes, UINT &offset, const char *opcode, UINT length)
{
	fprintf(m_stream, "%s\t0x", opcode);
	for (UINT i = 0; i < length; i++)
		fprintf(m_stream, "%02x", codeBytes[offset + i]);

	offset += length;
}

void PrintILMethodBody(ICorProfilerInfo3 *info, IMetaDataImport2 *import, ModuleID moduleId, FILE *m_stream, BYTE *codeBytes, UINT codeSize)
{
	for (UINT byte = 0; byte < codeSize; byte++)
	{
		switch (codeBytes[byte])
		{
		case 0x00:
			fprintf(m_stream, "nop");
			break;
		case 0x01:
			fprintf(m_stream, "break");
			break;
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
			fprintf(m_stream, "ldarg.%d", codeBytes[byte] - 0x02);
			break;
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			fprintf(m_stream, "ldloc.%d", codeBytes[byte] - 0x06);
			break;
		case 0x0A:
		case 0x0B:
		case 0x0C:
		case 0x0D:
			fprintf(m_stream, "stloc.%d", codeBytes[byte] - 0x0A);
			break;
		case 0x0E:
			PrintByteInstruction(m_stream, codeBytes, byte, "ldarg.s");
			break;
		case 0x0F:
			PrintByteInstruction(m_stream, codeBytes, byte, "ldarga.s");
			break;
		case 0x10:
			PrintByteInstruction(m_stream, codeBytes, byte, "starg.s");
			break;
		case 0x11:
			PrintByteInstruction(m_stream, codeBytes, byte, "ldloc.s");
			break;
		case 0x12:
			PrintByteInstruction(m_stream, codeBytes, byte, "ldloca.s");
			break;
		case 0x13:
			PrintByteInstruction(m_stream, codeBytes, byte, "stloc.s");
			break;
		case 0x14:
			fprintf(m_stream, "ldnull");
			break;
		case 0x15:
			fprintf(m_stream, "ldc.i4.m1");
			break;
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1E:
			fprintf(m_stream, "ldc.i4.%d", codeBytes[byte] - 0x16);
			break;
		case 0x1F:
			PrintByteInstruction(m_stream, codeBytes, byte, "ldc.i4.s");
			break;
		case 0x20:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldc.i4");
			break;
		case 0x21:
			PrintInt64Instruction(m_stream, codeBytes, byte, "ldc.i8");
			break;
		case 0x22:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldc.r4");
			break;
		case 0x23:
			PrintInt64Instruction(m_stream, codeBytes, byte, "ldc.r8");
			break;
		case 0x25:
			fprintf(m_stream, "dup");
			break;
		case 0x26:
			fprintf(m_stream, "pop");
			break;
		case 0x27:
			PrintInt32Instruction(m_stream, codeBytes, byte, "jmp");
			break;
		case 0x28:
			{
				mdMemberRef token = *(mdMethodSpec *) &codeBytes[byte + 1];
				PrintInt32Instruction(m_stream, codeBytes, byte, "call");

				mdToken classToken;
				if (import->GetMemberRefProps(token, &classToken, NULL, 0, NULL, NULL, NULL) != S_OK && import->GetMemberProps(token, &classToken, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != S_OK)
				{
					fprintf(m_stream, " - no method properties");
					break;
				}

				MDUTF8CSTR className = NULL;
				if (import->GetNameFromToken(classToken, &className) != S_OK)
				{
					fprintf(m_stream, " - no class name");
					break;
				}

				MDUTF8CSTR methodName = NULL;
				if (import->GetNameFromToken(token, &methodName) != S_OK)
				{
					fprintf(m_stream, " - no method name");
					break;
				}

				fprintf(m_stream, " - %s %s", className, methodName);
			}
			break;
		case 0x29:
			PrintInt32Instruction(m_stream, codeBytes, byte, "calli");
			break;
		case 0x2A:
			fprintf(m_stream, "ret");
			break;
		case 0x2B:
			PrintByteInstruction(m_stream, codeBytes, byte, "br.s");
			break;
		case 0x2C:
			PrintByteInstruction(m_stream, codeBytes, byte, "brfalse.s");
			break;
		case 0x2D:
			PrintByteInstruction(m_stream, codeBytes, byte, "brtrue.s");
			break;
		case 0x2E:
			PrintByteInstruction(m_stream, codeBytes, byte, "beq.s");
			break;
		case 0x2F:
			PrintByteInstruction(m_stream, codeBytes, byte, "bge.s");
			break;
		case 0x30:
			PrintByteInstruction(m_stream, codeBytes, byte, "bgt.s");
			break;
		case 0x31:
			PrintByteInstruction(m_stream, codeBytes, byte, "ble.s");
			break;
		case 0x32:
			PrintByteInstruction(m_stream, codeBytes, byte, "blt.s");
			break;
		case 0x33:
			PrintByteInstruction(m_stream, codeBytes, byte, "bne.un.s");
			break;
		case 0x34:
			PrintByteInstruction(m_stream, codeBytes, byte, "bge.un.s");
			break;
		case 0x35:
			PrintByteInstruction(m_stream, codeBytes, byte, "bgt.un.s");
			break;
		case 0x36:
			PrintByteInstruction(m_stream, codeBytes, byte, "ble.un.s");
			break;
		case 0x37:
			PrintByteInstruction(m_stream, codeBytes, byte, "blt.un.s");
			break;
		case 0x38:
			PrintInt32Instruction(m_stream, codeBytes, byte, "br");
			break;
		case 0x39:
			PrintInt32Instruction(m_stream, codeBytes, byte, "brfalse");
			break;
		case 0x3A:
			PrintInt32Instruction(m_stream, codeBytes, byte, "brtrue");
			break;
		case 0x3B:
			PrintInt32Instruction(m_stream, codeBytes, byte, "beq");
			break;
		case 0x3C:
			PrintInt32Instruction(m_stream, codeBytes, byte, "bge");
			break;
		case 0x3D:
			PrintInt32Instruction(m_stream, codeBytes, byte, "bgt");
			break;
		case 0x3E:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ble");
			break;
		case 0x3F:
			PrintInt32Instruction(m_stream, codeBytes, byte, "blt");
			break;
		case 0x40:
			PrintInt32Instruction(m_stream, codeBytes, byte, "bne.un");
			break;
		case 0x41:
			PrintInt32Instruction(m_stream, codeBytes, byte, "bge.un");
			break;
		case 0x42:
			PrintInt32Instruction(m_stream, codeBytes, byte, "bgt.un");
			break;
		case 0x43:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ble.un");
			break;
		case 0x44:
			PrintInt32Instruction(m_stream, codeBytes, byte, "blt.un");
			break;
		case 0x45:
			{
				UINT32 items = *((UINT32 *) (&codeBytes[byte + 1]));
				byte += 4;
				fprintf(m_stream, "switch\t0x%08x", items);
				for (UINT32 item = 0; item < items; item++)
				{
					INT32 value = *((INT32 *) (&codeBytes[byte + 1]));
					byte += 4;
					fprintf(m_stream, " 0x%08x", value);
				}
			}
			break;
		case 0x46:
			fprintf(m_stream, "ldind.i1");
			break;
		case 0x47:
			fprintf(m_stream, "ldind.u1");
			break;
		case 0x48:
			fprintf(m_stream, "ldind.i2");
			break;
		case 0x49:
			fprintf(m_stream, "ldind.u2");
			break;
		case 0x4A:
			fprintf(m_stream, "ldind.i4");
			break;
		case 0x4B:
			fprintf(m_stream, "ldind.u4");
			break;
		case 0x4C:
			fprintf(m_stream, "ldind.i8");
			break;
		case 0x4D:
			fprintf(m_stream, "ldind.i");
			break;
		case 0x4E:
			fprintf(m_stream, "ldind.r4");
			break;
		case 0x4F:
			fprintf(m_stream, "ldind.r8");
			break;
		case 0x50:
			fprintf(m_stream, "ldind.ref");
			break;
		case 0x51:
			fprintf(m_stream, "stind.ref");
			break;
		case 0x52:
			fprintf(m_stream, "stind.i1");
			break;
		case 0x53:
			fprintf(m_stream, "stind.i2");
			break;
		case 0x54:
			fprintf(m_stream, "stind.i4");
			break;
		case 0x55:
			fprintf(m_stream, "stind.i8");
			break;
		case 0x56:
			fprintf(m_stream, "stind.r4");
			break;
		case 0x57:
			fprintf(m_stream, "stind.r8");
			break;
		case 0x58:
			fprintf(m_stream, "add");
			break;
		case 0x59:
			fprintf(m_stream, "sub");
			break;
		case 0x5A:
			fprintf(m_stream, "mul");
			break;
		case 0x5B:
			fprintf(m_stream, "div");
			break;
		case 0x5C:
			fprintf(m_stream, "div.un");
			break;
		case 0x5D:
			fprintf(m_stream, "rem");
			break;
		case 0x5E:
			fprintf(m_stream, "rem.un");
			break;
		case 0x5F:
			fprintf(m_stream, "and");
			break;
		case 0x60:
			fprintf(m_stream, "or");
			break;
		case 0x61:
			fprintf(m_stream, "xor");
			break;
		case 0x62:
			fprintf(m_stream, "shl");
			break;
		case 0x63:
			fprintf(m_stream, "shr");
			break;
		case 0x64:
			fprintf(m_stream, "shr.un");
			break;
		case 0x65:
			fprintf(m_stream, "neg");
			break;
		case 0x66:
			fprintf(m_stream, "not");
			break;
		case 0x67:
			fprintf(m_stream, "conv.i1");
			break;
		case 0x68:
			fprintf(m_stream, "conv.i2");
			break;
		case 0x69:
			fprintf(m_stream, "conv.i4");
			break;
		case 0x6A:
			fprintf(m_stream, "conv.i8");
			break;
		case 0x6B:
			fprintf(m_stream, "conv.r4");
			break;
		case 0x6C:
			fprintf(m_stream, "conv.r8");
			break;
		case 0x6D:
			fprintf(m_stream, "conv.u4");
			break;
		case 0x6E:
			fprintf(m_stream, "conv.u8");
			break;
		case 0x6F:
			PrintInt32Instruction(m_stream, codeBytes, byte, "callvirt");
			break;
		case 0x70:
			PrintInt32Instruction(m_stream, codeBytes, byte, "cpobj");
			break;
		case 0x71:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldobj");
			break;
		case 0x72:
			{
				mdString token = *(mdString *) &codeBytes[byte + 1];
				WCHAR string[256];
				ULONG actualLength = 0;
				PrintInt32Instruction(m_stream, codeBytes, byte, "ldstr");
				if (import->GetUserString(token, string, 256, &actualLength) == S_OK)
				{
					if (actualLength < 256)
						string[actualLength] = 0;
					fprintf(m_stream, " - %S", string);
				}
			}
			break;
		case 0x73:
			PrintInt32Instruction(m_stream, codeBytes, byte, "newobj");
			break;
		case 0x74:
			PrintInt32Instruction(m_stream, codeBytes, byte, "castclass");
			break;
		case 0x75:
			PrintInt32Instruction(m_stream, codeBytes, byte, "isinst");
			break;
		case 0x76:
			fprintf(m_stream, "conv.r.un");
			break;
		case 0x79:
			PrintInt32Instruction(m_stream, codeBytes, byte, "unbox");
			break;
		case 0x7A:
			fprintf(m_stream, "throw");
			break;
		case 0x7B:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldfld");
			break;
		case 0x7C:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldflda");
			break;
		case 0x7D:
			PrintInt32Instruction(m_stream, codeBytes, byte, "stfld");
			break;
		case 0x7E:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldsfld");
			break;
		case 0x7F:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldsflda");
			break;
		case 0x80:
			PrintInt32Instruction(m_stream, codeBytes, byte, "stsfld");
			break;
		case 0x81:
			PrintInt32Instruction(m_stream, codeBytes, byte, "stobj");
			break;
		case 0x82:
			fprintf(m_stream, "conv.ovf.i1.un");
			break;
		case 0x83:
			fprintf(m_stream, "conv.ovf.i2.un");
			break;
		case 0x84:
			fprintf(m_stream, "conv.ovf.i4.un");
			break;
		case 0x85:
			fprintf(m_stream, "conv.ovf.i8.un");
			break;
		case 0x86:
			fprintf(m_stream, "conv.ovf.u1.un");
			break;
		case 0x87:
			fprintf(m_stream, "conv.ovf.u2.un");
			break;
		case 0x88:
			fprintf(m_stream, "conv.ovf.u4.un");
			break;
		case 0x89:
			fprintf(m_stream, "conv.ovf.u8.un");
			break;
		case 0x8A:
			fprintf(m_stream, "conv.ovf.i.un");
			break;
		case 0x8B:
			fprintf(m_stream, "conv.ovf.u.un");
			break;
		case 0x8C:
			PrintInt32Instruction(m_stream, codeBytes, byte, "box");
			break;
		case 0x8D:
			PrintInt32Instruction(m_stream, codeBytes, byte, "newarr");
			break;
		case 0x8E:
			fprintf(m_stream, "ldlen");
			break;
		case 0x8F:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldelema");
			break;
		case 0x90:
			fprintf(m_stream, "ldelem.i1");
			break;
		case 0x91:
			fprintf(m_stream, "ldelem.u1");
			break;
		case 0x92:
			fprintf(m_stream, "ldelem.i2");
			break;
		case 0x93:
			fprintf(m_stream, "ldelem.u2");
			break;
		case 0x94:
			fprintf(m_stream, "ldelem.i4");
			break;
		case 0x95:
			fprintf(m_stream, "ldelem.u4");
			break;
		case 0x96:
			fprintf(m_stream, "ldelem.i8");
			break;
		case 0x97:
			fprintf(m_stream, "ldelem.i");
			break;
		case 0x98:
			fprintf(m_stream, "ldelem.r4");
			break;
		case 0x99:
			fprintf(m_stream, "ldelem.r8");
			break;
		case 0x9A:
			fprintf(m_stream, "ldelem.ref");
			break;
		case 0x9B:
			fprintf(m_stream, "stelem.i");
			break;
		case 0x9C:
			fprintf(m_stream, "stelem.i1");
			break;
		case 0x9D:
			fprintf(m_stream, "stelem.i2");
			break;
		case 0x9E:
			fprintf(m_stream, "stelem.i4");
			break;
		case 0x9F:
			fprintf(m_stream, "stelem.i8");
			break;
		case 0xA0:
			fprintf(m_stream, "stelem.r4");
			break;
		case 0xA1:
			fprintf(m_stream, "stelem.r8");
			break;
		case 0xA2:
			fprintf(m_stream, "stelem.ref");
			break;
		case 0xA3:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldelem");
			break;
		case 0xA4:
			PrintInt32Instruction(m_stream, codeBytes, byte, "stelem");
			break;
		case 0xA5:
			PrintInt32Instruction(m_stream, codeBytes, byte, "unbox.any");
			break;
		case 0xB3:
			fprintf(m_stream, "conv.ovf.i1");
			break;
		case 0xB4:
			fprintf(m_stream, "conv.ovf.u1");
			break;
		case 0xB5:
			fprintf(m_stream, "conv.ovf.i2");
			break;
		case 0xB6:
			fprintf(m_stream, "conv.ovf.u2");
			break;
		case 0xB7:
			fprintf(m_stream, "conv.ovf.i4");
			break;
		case 0xB8:
			fprintf(m_stream, "conv.ovf.u4");
			break;
		case  0xB9:
			fprintf(m_stream, "conv.ovf.i8");
			break;
		case 0xBA:
			fprintf(m_stream, "conv.ovf.u8");
			break;
		case 0xC2:
			PrintInt32Instruction(m_stream, codeBytes, byte, "refanyval");
			break;
		case 0xC3:
			fprintf(m_stream, "ckfinite");
			break;
		case 0xC6:
			PrintInt32Instruction(m_stream, codeBytes, byte, "mkrefany");
			break;
		case 0xD0:
			PrintInt32Instruction(m_stream, codeBytes, byte, "ldtoken");
			break;
		case 0xD1:
			fprintf(m_stream, "conv.u2");
			break;
		case 0xD2:
			fprintf(m_stream, "conv.u1");
			break;
		case 0xD3:
			fprintf(m_stream, "conv.i");
			break;
		case 0xD4:
			fprintf(m_stream, "conv.ovf.i");
			break;
		case 0xD5:
			fprintf(m_stream, "conf.ovf.u");
			break;
		case 0xD6:
			fprintf(m_stream, "add.ovf");
			break;
		case 0xD7:
			fprintf(m_stream, "add.ovf.un");
			break;
		case 0xD8:
			fprintf(m_stream, "mul.ovf");
			break;
		case 0xD9:
			fprintf(m_stream, "mul.ovf.un");
			break;
		case 0xDA:
			fprintf(m_stream, "sub.ovf");
			break;
		case 0xDB:
			fprintf(m_stream, "sub.ovf.un");
			break;
		case 0xDC:
			fprintf(m_stream, "endfinally");
			break;
		case 0xDD:
			PrintInt32Instruction(m_stream, codeBytes, byte, "leave");
			break;
		case 0xDE:
			PrintByteInstruction(m_stream, codeBytes, byte, "leave.s");
			break;
		case 0xDF:
			fprintf(m_stream, "stind.i");
			break;
		case 0xE0:
			fprintf(m_stream, "conv.u");
			break;
		case 0xFE:
			byte++;
			switch(codeBytes[byte])
			{
			case 0x00:
				fprintf(m_stream, "arglist");
				break;
			case 0x01:
				fprintf(m_stream, "ceq");
				break;
			case 0x02:
				fprintf(m_stream, "cgt");
				break;
			case 0x03:
				fprintf(m_stream, "cgt.un");
				break;
			case 0x04:
				fprintf(m_stream, "clt");
				break;
			case 0x05:
				fprintf(m_stream, "clt.un");
				break;
			case 0x06:
				PrintInt32Instruction(m_stream, codeBytes, byte, "ldftn");
				break;
			case 0x07:
				PrintInt32Instruction(m_stream, codeBytes, byte, "ldvirtfn");
				break;
			case 0x09:
				PrintInt16Instruction(m_stream, codeBytes, byte, "ldarg");
				break;
			case 0x0A:
				PrintInt16Instruction(m_stream, codeBytes, byte, "ldarga");
				break;
			case 0x0B:
				PrintInt16Instruction(m_stream, codeBytes, byte, "starg");
				break;
			case 0x0C:
				PrintInt16Instruction(m_stream, codeBytes, byte, "ldloc");
				break;
			case 0x0D:
				PrintInt16Instruction(m_stream, codeBytes, byte, "ldloca");
				break;
			case 0x0E:
				PrintInt16Instruction(m_stream, codeBytes, byte, "stloc");
				break;
			case 0x0F:
				fprintf(m_stream, "localloc");
				break;
			case 0x11:
				fprintf(m_stream, "endfilter");
				break;
			case 0x12:
				fprintf(m_stream, "unaligned.");
				break;
			case 0x13:
				fprintf(m_stream, "volatile.");
				break;
			case 0x14:
				fprintf(m_stream, "tail.");
				break;
			case 0x15:
				PrintInt32Instruction(m_stream, codeBytes, byte, "initobj");
				break;
			case 0x16:
				fprintf(m_stream, "constrained.");
				break;
			case 0x17:
				fprintf(m_stream, "cpblk");
				break;
			case 0x18:
				fprintf(m_stream, "initblk");
				break;
			case 0x19:
				fprintf(m_stream, "no.");
				break;
			case 0x1A:
				fprintf(m_stream, "rethrow");
				break;
			case 0x1C:
				PrintInt32Instruction(m_stream, codeBytes, byte, "sizeof");
				break;
			case 0x1D:
				fprintf(m_stream, "refanytype");
				break;
			case 0x1E:
				fprintf(m_stream, "readonly.");
				break;
			default:
				fprintf(m_stream, "Unrecognized multi-byte opcode\t0x%02x", codeBytes[byte]);
				break;
			}
			break;
		default:
			fprintf(m_stream, "Unrecognized opcode\t0x%02x", codeBytes[byte]);
			break;
		}

		fprintf(m_stream, "\n");
	}
}

void PrintMethodInfo(ICorProfilerInfo3 *profilerInfo, ModuleID moduleId, mdMethodDef methodId, FILE *stream, IMetaDataImport2 **outMetadata)
{
	if (outMetadata != NULL)
		*outMetadata = NULL;

	LPCBYTE baseLoadAddress = NULL;
	ULONG actualLength;
	WCHAR moduleName[256];
	AssemblyID assemblyID = NULL;
	HRESULT result = profilerInfo->GetModuleInfo(moduleId, &baseLoadAddress, 256, &actualLength, moduleName, &assemblyID);
	if (result != S_OK)
		return;

	ULONG actualAssemblyNameLength;
	WCHAR assemblyName[256];
	AppDomainID appDomainId = NULL;
	ModuleID unusedModuleId = NULL;
	result = profilerInfo->GetAssemblyInfo(assemblyID, 256, &actualAssemblyNameLength, assemblyName, &appDomainId, &unusedModuleId);
	if (result != S_OK)
		return;

	FunctionID functionId = NULL;
	result = profilerInfo->GetFunctionFromToken(moduleId, methodId, &functionId);
	if (result != S_OK)
		return;

	ClassID classId = NULL;
	mdTypeDef classToken = 0;
	mdToken unusedToken;
	result = profilerInfo->GetFunctionInfo(functionId, &classId, &unusedModuleId, &unusedToken);
	if (result != S_OK || classId == NULL)
		return;

	result = profilerInfo->GetClassIDInfo(classId, &unusedModuleId, &classToken);
	if (result != S_OK)
		return;

	IMetaDataImport2 *metadata = NULL;
	result = profilerInfo->GetModuleMetaData(moduleId, ofRead, IID_IMetaDataImport2, (IUnknown **) &metadata);
	if (result != S_OK || metadata == NULL)
		return;

	if (outMetadata != NULL)
		*outMetadata = metadata;

	MDUTF8CSTR className = NULL;
	result = metadata->GetNameFromToken(classToken, &className);
	if (result != S_OK || className == NULL)
		return;

	MDUTF8CSTR methodName = NULL;
	result = metadata->GetNameFromToken(methodId, &methodName);
	if (result != S_OK || methodName == NULL)
		return;

	fprintf(stream, "Assembly: %S, Module: %S, Class: %s, Method name: %s\n", assemblyName, moduleName, className, methodName);
}

HRESULT ProfilerInfoShim::GetILFunctionBody(ModuleID moduleId, mdMethodDef methodId, LPCBYTE *ppMethodHeader, ULONG *pcbMethodSize)
{
	HRESULT result = m_wrappedInfo->GetILFunctionBody(moduleId, methodId, ppMethodHeader, pcbMethodSize);
	if (result != S_OK)
		return result;

	IMAGE_COR_ILMETHOD *method = (IMAGE_COR_ILMETHOD *)*ppMethodHeader;
	COR_ILMETHOD_FAT *fatImage = (COR_ILMETHOD_FAT *)&method->Fat;
	UINT codeSize = 0;
	BYTE *codeBytes = NULL;
	if (fatImage->IsFat())
	{
		codeSize = fatImage->GetCodeSize();
		codeBytes = fatImage->GetCode();
	}
	else
	{
		COR_ILMETHOD_TINY *tinyImage = (COR_ILMETHOD_TINY *)&method->Tiny;
		codeSize = tinyImage->GetCodeSize();
		codeBytes = tinyImage->GetCode();
	}

	fprintf(m_stream, "Getting IL for ");
	IMetaDataImport2 *metadata = NULL;
	PrintMethodInfo(m_wrappedInfo, moduleId, methodId, m_stream, &metadata);

	if (codeBytes != NULL)
		PrintILMethodBody(m_wrappedInfo, metadata, moduleId, m_stream, codeBytes, codeSize);
	fflush(m_stream);

	metadata->Release();

	return result;
}

HRESULT ProfilerInfoShim::GetILFunctionBodyAllocator(ModuleID moduleId, IMethodMalloc **ppMalloc)
{
	return m_wrappedInfo->GetILFunctionBodyAllocator(moduleId, ppMalloc);
}

HRESULT ProfilerInfoShim::SetILFunctionBody(ModuleID moduleId, mdMethodDef methodid, LPCBYTE pbNewILMethodHeader)
{
	HRESULT result = m_wrappedInfo->SetILFunctionBody(moduleId, methodid, pbNewILMethodHeader);
	if (result != S_OK)
		return result;

	IMAGE_COR_ILMETHOD *method = (IMAGE_COR_ILMETHOD *)pbNewILMethodHeader;
	COR_ILMETHOD_FAT *fatImage = (COR_ILMETHOD_FAT *)&method->Fat;
	UINT codeSize = 0;
	BYTE *codeBytes = NULL;
	if (fatImage->IsFat())
	{
		codeSize = fatImage->GetCodeSize();
		codeBytes = fatImage->GetCode();
	}
	else
	{
		COR_ILMETHOD_TINY *tinyImage = (COR_ILMETHOD_TINY *)&method->Tiny;
		codeSize = tinyImage->GetCodeSize();
		codeBytes = tinyImage->GetCode();
	}

	fprintf(m_stream, "Setting IL for ", moduleId, methodid);
	IMetaDataImport2 *metadata = NULL;
	PrintMethodInfo(m_wrappedInfo, moduleId, methodid, m_stream, &metadata);
	if (codeBytes != NULL)
		PrintILMethodBody(m_wrappedInfo, metadata, moduleId, m_stream, codeBytes, codeSize);

	fflush(m_stream);

	metadata->Release();

	return result;
}

HRESULT ProfilerInfoShim::GetAppDomainInfo(AppDomainID appDomainId, ULONG cchName, ULONG *pcchName, WCHAR szName[  ], ProcessID *pProcessId)
{
	fprintf(m_stream, "Call GetAppDomainInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetAppDomainInfo(appDomainId, cchName, pcchName, szName, pProcessId);
}

HRESULT ProfilerInfoShim::GetAssemblyInfo(AssemblyID assemblyId, ULONG cchName, ULONG *pcchName, WCHAR szName[  ], AppDomainID *pAppDomainId, ModuleID *pModuleId)
{
	HRESULT result = m_wrappedInfo->GetAssemblyInfo(assemblyId, cchName, pcchName, szName, pAppDomainId, pModuleId);
	if (result != S_OK)
		return result;

	return result;
}

HRESULT ProfilerInfoShim::SetFunctionReJIT(FunctionID functionId)
{
	fprintf(m_stream, "Call SetFunctionReJIT\n");
	fflush(m_stream);
	return m_wrappedInfo->SetFunctionReJIT(functionId);
}

HRESULT ProfilerInfoShim::ForceGC()
{
	fprintf(m_stream, "Call ForceGC\n");
	fflush(m_stream);
	return m_wrappedInfo->ForceGC();
}

HRESULT ProfilerInfoShim::SetILInstrumentedCodeMap(FunctionID functionId, BOOL fStartJit, ULONG cILMapEntries, COR_IL_MAP rgILMapEntries[  ])
{
	fprintf(m_stream, "Call SetILInstrumentedCodeMap\n");
	fflush(m_stream);
	return m_wrappedInfo->SetILInstrumentedCodeMap(functionId, fStartJit, cILMapEntries, rgILMapEntries);
}

HRESULT ProfilerInfoShim::GetInprocInspectionInterface(IUnknown **ppicd)
{
	fprintf(m_stream, "Call GetInprocInspectionInterface\n");
	fflush(m_stream);
	return m_wrappedInfo->GetInprocInspectionInterface(ppicd);
}

HRESULT ProfilerInfoShim::GetInprocInspectionIThisThread(IUnknown **ppicd)
{
	fprintf(m_stream, "Call GetInprocInspectionIThisThread\n");
	fflush(m_stream);
	return m_wrappedInfo->GetInprocInspectionIThisThread(ppicd);
}

HRESULT ProfilerInfoShim::GetThreadContext(ThreadID threadId, ContextID *pContextId)
{
	fprintf(m_stream, "Call GetThreadContext\n");
	fflush(m_stream);
	return m_wrappedInfo->GetThreadContext(threadId, pContextId);
}

HRESULT ProfilerInfoShim::BeginInprocDebugging(BOOL fThisThreadOnly, DWORD *pdwProfilerContext)
{
	fprintf(m_stream, "Call BeginInprocDebugging\n");
	fflush(m_stream);
	return m_wrappedInfo->BeginInprocDebugging(fThisThreadOnly, pdwProfilerContext);
}

HRESULT ProfilerInfoShim::EndInprocDebugging(DWORD dwProfilerContext)
{
	fprintf(m_stream, "Call EndInprocDebugging\n");
	fflush(m_stream);
	return m_wrappedInfo->EndInprocDebugging(dwProfilerContext);
}

HRESULT ProfilerInfoShim::GetILToNativeMapping(FunctionID functionId, ULONG32 cMap, ULONG32 *pcMap, COR_DEBUG_IL_TO_NATIVE_MAP map[  ])
{
	fprintf(m_stream, "Call GetILToNativeMapping\n");
	fflush(m_stream);
	return m_wrappedInfo->GetILToNativeMapping(functionId, cMap, pcMap, map);
}

HRESULT ProfilerInfoShim::DoStackSnapshot(ThreadID thread, StackSnapshotCallback *callback, ULONG32 infoFlags, void *clientData, BYTE context[  ], ULONG32 contextSize)
{
	fprintf(m_stream, "Call DoStackSnapshot\n");
	fflush(m_stream);
	return m_wrappedInfo->DoStackSnapshot(thread, callback, infoFlags, clientData, context, contextSize);
}

HRESULT ProfilerInfoShim::SetEnterLeaveFunctionHooks2(FunctionEnter2 *pFuncEnter, FunctionLeave2 *pFuncLeave, FunctionTailcall2 *pFuncTailcall)
{
	fprintf(m_stream, "Call SetEnterLeaveFunctionHooks2\n");
	fflush(m_stream);
	return m_wrappedInfo->SetEnterLeaveFunctionHooks2(pFuncEnter, pFuncLeave, pFuncTailcall);
}

HRESULT ProfilerInfoShim::GetFunctionInfo2(FunctionID funcId, COR_PRF_FRAME_INFO frameInfo, ClassID *pClassId, ModuleID *pModuleId, mdToken *pToken, ULONG32 cTypeArgs, ULONG32 *pcTypeArgs, ClassID typeArgs[  ])
{
	fprintf(m_stream, "Call GetFunctionInfo2\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionInfo2(funcId, frameInfo, pClassId, pModuleId, pToken, cTypeArgs, pcTypeArgs, typeArgs);
}

HRESULT ProfilerInfoShim::GetStringLayout(ULONG *pBufferLengthOffset, ULONG *pStringLengthOffset, ULONG *pBufferOffset)
{
	fprintf(m_stream, "Call GetStringLayout\n");
	fflush(m_stream);
	return m_wrappedInfo->GetStringLayout(pBufferLengthOffset, pStringLengthOffset, pBufferOffset);
}

HRESULT ProfilerInfoShim::GetClassLayout(ClassID classID, COR_FIELD_OFFSET rFieldOffset[  ], ULONG cFieldOffset, ULONG *pcFieldOffset, ULONG *pulClassSize)
{
	fprintf(m_stream, "Call GetClassLayout\n");
	fflush(m_stream);
	return m_wrappedInfo->GetClassLayout(classID, rFieldOffset, cFieldOffset, pcFieldOffset, pulClassSize);
}

HRESULT ProfilerInfoShim::GetClassIDInfo2(ClassID classId, ModuleID *pModuleId, mdTypeDef *pTypeDefToken, ClassID *pParentClassId, ULONG32 cNumTypeArgs, ULONG32 *pcNumTypeArgs, ClassID typeArgs[  ])
{
	fprintf(m_stream, "Call GetClassIDInfo2\n");
	fflush(m_stream);
	return m_wrappedInfo->GetClassIDInfo2(classId, pModuleId, pTypeDefToken, pParentClassId, cNumTypeArgs, pcNumTypeArgs, typeArgs);
}

HRESULT ProfilerInfoShim::GetCodeInfo2(FunctionID functionID, ULONG32 cCodeInfos, ULONG32 *pcCodeInfos, COR_PRF_CODE_INFO codeInfos[  ])
{
	fprintf(m_stream, "Call GetCodeInfo2\n");
	fflush(m_stream);
	return m_wrappedInfo->GetCodeInfo2(functionID, cCodeInfos, pcCodeInfos, codeInfos);
}

HRESULT ProfilerInfoShim::GetClassFromTokenAndTypeArgs(ModuleID moduleID, mdTypeDef typeDef, ULONG32 cTypeArgs, ClassID typeArgs[  ], ClassID *pClassID)
{
	fprintf(m_stream, "Call GetClassFromTokenAndTypeArgs\n");
	fflush(m_stream);
	return m_wrappedInfo->GetClassFromTokenAndTypeArgs(moduleID, typeDef, cTypeArgs, typeArgs, pClassID);
}

HRESULT ProfilerInfoShim::GetFunctionFromTokenAndTypeArgs(ModuleID moduleID, mdMethodDef funcDef, ClassID classId, ULONG32 cTypeArgs, ClassID typeArgs[  ], FunctionID *pFunctionID)
{
	fprintf(m_stream, "Call GetFunctionFromTokenAndTypeArgs\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionFromTokenAndTypeArgs(moduleID, funcDef, classId, cTypeArgs, typeArgs, pFunctionID);
}

HRESULT ProfilerInfoShim::EnumModuleFrozenObjects(ModuleID moduleID, ICorProfilerObjectEnum **ppEnum)
{
	fprintf(m_stream, "Call EnumModuleFrozenObjects\n");
	fflush(m_stream);
	return m_wrappedInfo->EnumModuleFrozenObjects(moduleID, ppEnum);
}

HRESULT ProfilerInfoShim::GetArrayObjectInfo(ObjectID objectId, ULONG32 cDimensions, ULONG32 pDimensionSizes[  ], int pDimensionLowerBounds[  ], BYTE **ppData)
{
	fprintf(m_stream, "Call GetArrayObjectInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetArrayObjectInfo(objectId, cDimensions, pDimensionSizes, pDimensionLowerBounds, ppData);
}

HRESULT ProfilerInfoShim::GetBoxClassLayout(ClassID classId, ULONG32 *pBufferOffset)
{
	fprintf(m_stream, "Call GetBoxClassLayout\n");
	fflush(m_stream);
	return m_wrappedInfo->GetBoxClassLayout(classId, pBufferOffset);
}

HRESULT ProfilerInfoShim::GetThreadAppDomain(ThreadID threadId, AppDomainID *pAppDomainId)
{
	fprintf(m_stream, "Call GetThreadAppDomain\n");
	fflush(m_stream);
	return m_wrappedInfo->GetThreadAppDomain(threadId, pAppDomainId);
}

HRESULT ProfilerInfoShim::GetRVAStaticAddress(ClassID classId, mdFieldDef fieldToken, void **ppAddress)
{
	fprintf(m_stream, "Call GetRVAStaticAddress\n");
	fflush(m_stream);
	return m_wrappedInfo->GetRVAStaticAddress(classId, fieldToken, ppAddress);
}

HRESULT ProfilerInfoShim::GetAppDomainStaticAddress(ClassID classId, mdFieldDef fieldToken, AppDomainID appDomainId, void **ppAddress)
{
	fprintf(m_stream, "Call GetAppDomainStaticAddress\n");
	fflush(m_stream);
	return m_wrappedInfo->GetAppDomainStaticAddress(classId, fieldToken, appDomainId, ppAddress);
}

HRESULT ProfilerInfoShim::GetThreadStaticAddress(ClassID classId, mdFieldDef fieldToken, ThreadID threadId, void **ppAddress)
{
	fprintf(m_stream, "Call GetThreadStaticAddress\n");
	fflush(m_stream);
	return m_wrappedInfo->GetThreadStaticAddress(classId, fieldToken, threadId, ppAddress);
}

HRESULT ProfilerInfoShim::GetContextStaticAddress(ClassID classId, mdFieldDef fieldToken, ContextID contextId, void **ppAddress)
{
	fprintf(m_stream, "Call GetContextStaticAddress\n");
	fflush(m_stream);
	return m_wrappedInfo->GetContextStaticAddress(classId, fieldToken, contextId, ppAddress);
}

HRESULT ProfilerInfoShim::GetStaticFieldInfo(ClassID classId, mdFieldDef fieldToken, COR_PRF_STATIC_TYPE *pFieldInfo)
{
	fprintf(m_stream, "Call GetStaticFieldInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetStaticFieldInfo(classId, fieldToken, pFieldInfo);
}

HRESULT ProfilerInfoShim::GetGenerationBounds(ULONG cObjectRanges, ULONG *pcObjectRanges, COR_PRF_GC_GENERATION_RANGE ranges[  ])
{
	fprintf(m_stream, "Call GetGenerationBounds\n");
	fflush(m_stream);
	return m_wrappedInfo->GetGenerationBounds(cObjectRanges, pcObjectRanges, ranges);
}

HRESULT ProfilerInfoShim::GetObjectGeneration(ObjectID objectId, COR_PRF_GC_GENERATION_RANGE *range)
{
	fprintf(m_stream, "Call GetObjectGeneration\n");
	fflush(m_stream);
	return m_wrappedInfo->GetObjectGeneration(objectId, range);
}

HRESULT ProfilerInfoShim::GetNotifiedExceptionClauseInfo(COR_PRF_EX_CLAUSE_INFO *pinfo)
{
	fprintf(m_stream, "Call GetNotifiedExceptionClauseInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->GetNotifiedExceptionClauseInfo(pinfo);
}

HRESULT ProfilerInfoShim::EnumJITedFunctions(ICorProfilerFunctionEnum **ppEnum)
{
	fprintf(m_stream, "Call EnumJITedFunctions\n");
	fflush(m_stream);
	return m_wrappedInfo->EnumJITedFunctions(ppEnum);
}

HRESULT ProfilerInfoShim::RequestProfilerDetach(DWORD dwExpectedCompletionMilliseconds)
{
	fprintf(m_stream, "Call RequestProfilerDetach\n");
	fflush(m_stream);
	return m_wrappedInfo->RequestProfilerDetach(dwExpectedCompletionMilliseconds);
}

HRESULT ProfilerInfoShim::SetFunctionIDMapper2(FunctionIDMapper2 *pFunc, void *clientData)
{
	fprintf(m_stream, "Call SetFunctionIDMapper2\n");
	fflush(m_stream);
	return m_wrappedInfo->SetFunctionIDMapper2(pFunc, clientData);
}

HRESULT ProfilerInfoShim::GetStringLayout2(ULONG *pStringLengthOffset, ULONG *pBufferOffset)
{
	fprintf(m_stream, "Call GetStringLayout2\n");
	fflush(m_stream);
	return m_wrappedInfo->GetStringLayout2(pStringLengthOffset, pBufferOffset);
}

HRESULT ProfilerInfoShim::SetEnterLeaveFunctionHooks3(FunctionEnter3 *pFuncEnter3, FunctionLeave3 *pFuncLeave3, FunctionTailcall3 *pFuncTailcall3)
{
	fprintf(m_stream, "Call SetEnterLeaveFunctionHooks3\n");
	fflush(m_stream);
	return m_wrappedInfo->SetEnterLeaveFunctionHooks3(pFuncEnter3, pFuncLeave3, pFuncTailcall3);
}

HRESULT ProfilerInfoShim::SetEnterLeaveFunctionHooks3WithInfo(FunctionEnter3WithInfo *pFuncEnter3WithInfo, FunctionLeave3WithInfo *pFuncLeave3WithInfo, FunctionTailcall3WithInfo *pFuncTailcall3WithInfo)
{
	fprintf(m_stream, "Call SetEnterLeaveFunctionHooks3WithInfo\n");
	fflush(m_stream);
	return m_wrappedInfo->SetEnterLeaveFunctionHooks3WithInfo(pFuncEnter3WithInfo, pFuncLeave3WithInfo, pFuncTailcall3WithInfo);
}

HRESULT ProfilerInfoShim::GetFunctionEnter3Info(FunctionID functionId, COR_PRF_ELT_INFO eltInfo, COR_PRF_FRAME_INFO *pFrameInfo, ULONG *pcbArgumentInfo, COR_PRF_FUNCTION_ARGUMENT_INFO *pArgumentInfo)
{
	fprintf(m_stream, "Call GetFunctionEnter3Info\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionEnter3Info(functionId, eltInfo, pFrameInfo, pcbArgumentInfo, pArgumentInfo);
}

HRESULT ProfilerInfoShim::GetFunctionLeave3Info(FunctionID functionId, COR_PRF_ELT_INFO eltInfo, COR_PRF_FRAME_INFO *pFrameInfo, COR_PRF_FUNCTION_ARGUMENT_RANGE *pRetvalRange)
{
	fprintf(m_stream, "Call GetFunctionLeave3Info\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionLeave3Info(functionId, eltInfo, pFrameInfo, pRetvalRange);
}

HRESULT ProfilerInfoShim::GetFunctionTailcall3Info(FunctionID functionId, COR_PRF_ELT_INFO eltInfo, COR_PRF_FRAME_INFO *pFrameInfo)
{
	fprintf(m_stream, "Call GetFunctionTailcall3Info\n");
	fflush(m_stream);
	return m_wrappedInfo->GetFunctionTailcall3Info(functionId, eltInfo, pFrameInfo);
}

HRESULT ProfilerInfoShim::EnumModules(ICorProfilerModuleEnum **ppEnum)
{
	fprintf(m_stream, "Call EnumModules\n");
	fflush(m_stream);
	return m_wrappedInfo->EnumModules(ppEnum);
}

HRESULT ProfilerInfoShim::GetRuntimeInformation(USHORT *pClrInstanceId, COR_PRF_RUNTIME_TYPE *pRuntimeType, USHORT *pMajorVersion, USHORT *pMinorVersion, USHORT *pBuildNumber, USHORT *pQFEVersion, ULONG cchVersionString, ULONG *pcchVersionString, WCHAR szVersionString[  ])
{
	fprintf(m_stream, "Call GetRuntimeInformation\n");
	fflush(m_stream);
	return m_wrappedInfo->GetRuntimeInformation(pClrInstanceId, pRuntimeType, pMajorVersion, pMinorVersion, pBuildNumber, pQFEVersion, cchVersionString, pcchVersionString, szVersionString);
}

HRESULT ProfilerInfoShim::GetThreadStaticAddress2(ClassID classId, mdFieldDef fieldToken, AppDomainID appDomainId, ThreadID threadId, void **ppAddress)
{
	fprintf(m_stream, "Call GetThreadStaticAddress2\n");
	fflush(m_stream);
	return m_wrappedInfo->GetThreadStaticAddress2(classId, fieldToken, appDomainId, threadId, ppAddress);
}

HRESULT ProfilerInfoShim::GetAppDomainsContainingModule(ModuleID moduleId, ULONG32 cAppDomainIds, ULONG32 *pcAppDomainIds, AppDomainID appDomainIds[  ])
{
	fprintf(m_stream, "Call GetAppDomainsContainingModule\n");
	fflush(m_stream);
	return m_wrappedInfo->GetAppDomainsContainingModule(moduleId, cAppDomainIds, pcAppDomainIds, appDomainIds);
}

HRESULT ProfilerInfoShim::GetModuleInfo2(ModuleID moduleId, LPCBYTE *ppBaseLoadAddress, ULONG cchName, ULONG *pcchName, WCHAR szName[  ], AssemblyID *pAssemblyId, DWORD *pdwModuleFlags)
{
	fprintf(m_stream, "Call GetModuleInfo2\n");
	fflush(m_stream);
	return m_wrappedInfo->GetModuleInfo2(moduleId, ppBaseLoadAddress, cchName, pcchName, szName, pAssemblyId, pdwModuleFlags);
}
