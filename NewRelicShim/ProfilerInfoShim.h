#pragma once

#include "mscoree.h"
#include "basehlp.h"

class ProfilerInfoShim :
	public ICorProfilerInfo3
{
public:
	ProfilerInfoShim(ICorProfilerInfo3 *wrappedInfo, FILE *stream);
	~ProfilerInfoShim();
	COM_METHOD(ULONG) AddRef();
	COM_METHOD(ULONG) Release();
	COM_METHOD(HRESULT) QueryInterface(REFIID riid, void **ppInterface);
	COM_METHOD(HRESULT) GetClassFromObject(ObjectID objectId, ClassID *pClassId);
	COM_METHOD(HRESULT) GetClassFromToken(ModuleID moduleId, mdTypeDef typeDef, ClassID *pClassId);
	COM_METHOD(HRESULT) GetCodeInfo(FunctionID functionId, LPCBYTE *pStart, ULONG *pcSize);
	COM_METHOD(HRESULT) GetEventMask(DWORD *pdwEvents);
	COM_METHOD(HRESULT) GetFunctionFromIP(LPCBYTE ip, FunctionID *pFunctionId);
	COM_METHOD(HRESULT) GetFunctionFromToken(ModuleID moduleId, mdToken token, FunctionID *pFunctionId);
	COM_METHOD(HRESULT) GetHandleFromThread(ThreadID threadId, HANDLE *phThread);
	COM_METHOD(HRESULT) GetObjectSize(ObjectID objectId, ULONG *pcSize);
	COM_METHOD(HRESULT) IsArrayClass(ClassID classId, CorElementType *pBaseElemType, ClassID *pBaseClassId, ULONG *pcRank);
	COM_METHOD(HRESULT) GetThreadInfo(ThreadID threadId, DWORD *pdwWin32ThreadId);
	COM_METHOD(HRESULT) GetCurrentThreadID(ThreadID *pThreadId);
	COM_METHOD(HRESULT) GetClassIDInfo(ClassID classId, ModuleID *pModuleId, mdTypeDef *pTypeDefToken);
	COM_METHOD(HRESULT) GetFunctionInfo(FunctionID functionId, ClassID *pClassId, ModuleID *pModuleId, mdToken *pToken);
	COM_METHOD(HRESULT) SetEventMask(DWORD dwEvents);
	COM_METHOD(HRESULT) SetEnterLeaveFunctionHooks(FunctionEnter *pFuncEnter, FunctionLeave *pFuncLeave, FunctionTailcall *pFuncTailcall);
	COM_METHOD(HRESULT) SetFunctionIDMapper(FunctionIDMapper *pFunc);
	COM_METHOD(HRESULT) GetTokenAndMetaDataFromFunction(FunctionID functionId, REFIID riid, IUnknown **ppImport, mdToken *pToken);
	COM_METHOD(HRESULT) GetModuleInfo(ModuleID moduleId, LPCBYTE *ppBaseLoadAddress, ULONG cchName, ULONG *pcchName, _Out_writes_to_(cchName, *pcchName)  WCHAR szName[  ], AssemblyID *pAssemblyId);
	COM_METHOD(HRESULT) GetModuleMetaData(ModuleID moduleId, DWORD dwOpenFlags, REFIID riid, IUnknown **ppOut);
	COM_METHOD(HRESULT) GetILFunctionBody(ModuleID moduleId, mdMethodDef methodId, LPCBYTE *ppMethodHeader, ULONG *pcbMethodSize);
	COM_METHOD(HRESULT) GetILFunctionBodyAllocator(ModuleID moduleId, IMethodMalloc **ppMalloc);
	COM_METHOD(HRESULT) SetILFunctionBody(ModuleID moduleId, mdMethodDef methodid, LPCBYTE pbNewILMethodHeader);
	COM_METHOD(HRESULT) GetAppDomainInfo(AppDomainID appDomainId, ULONG cchName, ULONG *pcchName, _Out_writes_to_(cchName, *pcchName)  WCHAR szName[  ], ProcessID *pProcessId);
	COM_METHOD(HRESULT) GetAssemblyInfo(AssemblyID assemblyId, ULONG cchName, ULONG *pcchName, _Out_writes_to_(cchName, *pcchName)  WCHAR szName[  ], AppDomainID *pAppDomainId, ModuleID *pModuleId);
	COM_METHOD(HRESULT) SetFunctionReJIT(FunctionID functionId);
	COM_METHOD(HRESULT) ForceGC(void);
	COM_METHOD(HRESULT) SetILInstrumentedCodeMap(FunctionID functionId, BOOL fStartJit, ULONG cILMapEntries, COR_IL_MAP rgILMapEntries[  ]);
	COM_METHOD(HRESULT) GetInprocInspectionInterface(IUnknown **ppicd);
	COM_METHOD(HRESULT) GetInprocInspectionIThisThread(IUnknown **ppicd);
	COM_METHOD(HRESULT) GetThreadContext(ThreadID threadId, ContextID *pContextId);
	COM_METHOD(HRESULT) BeginInprocDebugging(BOOL fThisThreadOnly, DWORD *pdwProfilerContext);
	COM_METHOD(HRESULT) EndInprocDebugging(DWORD dwProfilerContext);
	COM_METHOD(HRESULT) GetILToNativeMapping(FunctionID functionId, ULONG32 cMap, ULONG32 *pcMap, COR_DEBUG_IL_TO_NATIVE_MAP map[  ]);
	COM_METHOD(HRESULT) DoStackSnapshot(ThreadID thread, StackSnapshotCallback *callback, ULONG32 infoFlags, void *clientData, BYTE context[  ], ULONG32 contextSize);
	COM_METHOD(HRESULT) SetEnterLeaveFunctionHooks2(FunctionEnter2 *pFuncEnter, FunctionLeave2 *pFuncLeave, FunctionTailcall2 *pFuncTailcall);
	COM_METHOD(HRESULT) GetFunctionInfo2(FunctionID funcId, COR_PRF_FRAME_INFO frameInfo, ClassID *pClassId, ModuleID *pModuleId, mdToken *pToken, ULONG32 cTypeArgs, ULONG32 *pcTypeArgs, ClassID typeArgs[  ]);
	COM_METHOD(HRESULT) GetStringLayout(ULONG *pBufferLengthOffset, ULONG *pStringLengthOffset, ULONG *pBufferOffset);
	COM_METHOD(HRESULT) GetClassLayout(ClassID classID, COR_FIELD_OFFSET rFieldOffset[  ], ULONG cFieldOffset, ULONG *pcFieldOffset, ULONG *pulClassSize);
	COM_METHOD(HRESULT) GetClassIDInfo2(ClassID classId, ModuleID *pModuleId, mdTypeDef *pTypeDefToken, ClassID *pParentClassId, ULONG32 cNumTypeArgs, ULONG32 *pcNumTypeArgs, ClassID typeArgs[  ]);
	COM_METHOD(HRESULT) GetCodeInfo2(FunctionID functionID, ULONG32 cCodeInfos, ULONG32 *pcCodeInfos, COR_PRF_CODE_INFO codeInfos[  ]);
	COM_METHOD(HRESULT) GetClassFromTokenAndTypeArgs(ModuleID moduleID, mdTypeDef typeDef, ULONG32 cTypeArgs, ClassID typeArgs[  ], ClassID *pClassID);
	COM_METHOD(HRESULT) GetFunctionFromTokenAndTypeArgs(ModuleID moduleID, mdMethodDef funcDef, ClassID classId, ULONG32 cTypeArgs, ClassID typeArgs[  ], FunctionID *pFunctionID);
	COM_METHOD(HRESULT) EnumModuleFrozenObjects(ModuleID moduleID, ICorProfilerObjectEnum **ppEnum);
	COM_METHOD(HRESULT) GetArrayObjectInfo(ObjectID objectId, ULONG32 cDimensions, ULONG32 pDimensionSizes[  ], int pDimensionLowerBounds[  ], BYTE **ppData);
	COM_METHOD(HRESULT) GetBoxClassLayout(ClassID classId, ULONG32 *pBufferOffset);
	COM_METHOD(HRESULT) GetThreadAppDomain(ThreadID threadId, AppDomainID *pAppDomainId);
	COM_METHOD(HRESULT) GetRVAStaticAddress(ClassID classId, mdFieldDef fieldToken, void **ppAddress);
	COM_METHOD(HRESULT) GetAppDomainStaticAddress(ClassID classId, mdFieldDef fieldToken, AppDomainID appDomainId, void **ppAddress);
	COM_METHOD(HRESULT) GetThreadStaticAddress(ClassID classId, mdFieldDef fieldToken, ThreadID threadId, void **ppAddress);
	COM_METHOD(HRESULT) GetContextStaticAddress(ClassID classId, mdFieldDef fieldToken, ContextID contextId, void **ppAddress);
	COM_METHOD(HRESULT) GetStaticFieldInfo(ClassID classId, mdFieldDef fieldToken, COR_PRF_STATIC_TYPE *pFieldInfo);
	COM_METHOD(HRESULT) GetGenerationBounds(ULONG cObjectRanges, ULONG *pcObjectRanges, COR_PRF_GC_GENERATION_RANGE ranges[  ]);
	COM_METHOD(HRESULT) GetObjectGeneration(ObjectID objectId, COR_PRF_GC_GENERATION_RANGE *range);
	COM_METHOD(HRESULT) GetNotifiedExceptionClauseInfo(COR_PRF_EX_CLAUSE_INFO *pinfo);
	COM_METHOD(HRESULT) EnumJITedFunctions(ICorProfilerFunctionEnum **ppEnum);
	COM_METHOD(HRESULT) RequestProfilerDetach(DWORD dwExpectedCompletionMilliseconds);
	COM_METHOD(HRESULT) SetFunctionIDMapper2(FunctionIDMapper2 *pFunc, void *clientData);
	COM_METHOD(HRESULT) GetStringLayout2(ULONG *pStringLengthOffset, ULONG *pBufferOffset);
	COM_METHOD(HRESULT) SetEnterLeaveFunctionHooks3(FunctionEnter3 *pFuncEnter3, FunctionLeave3 *pFuncLeave3, FunctionTailcall3 *pFuncTailcall3);
	COM_METHOD(HRESULT) SetEnterLeaveFunctionHooks3WithInfo(FunctionEnter3WithInfo *pFuncEnter3WithInfo, FunctionLeave3WithInfo *pFuncLeave3WithInfo, FunctionTailcall3WithInfo *pFuncTailcall3WithInfo);
	COM_METHOD(HRESULT) GetFunctionEnter3Info(FunctionID functionId, COR_PRF_ELT_INFO eltInfo, COR_PRF_FRAME_INFO *pFrameInfo, ULONG *pcbArgumentInfo, COR_PRF_FUNCTION_ARGUMENT_INFO *pArgumentInfo);
	COM_METHOD(HRESULT) GetFunctionLeave3Info(FunctionID functionId, COR_PRF_ELT_INFO eltInfo, COR_PRF_FRAME_INFO *pFrameInfo, COR_PRF_FUNCTION_ARGUMENT_RANGE *pRetvalRange);
	COM_METHOD(HRESULT) GetFunctionTailcall3Info(FunctionID functionId, COR_PRF_ELT_INFO eltInfo, COR_PRF_FRAME_INFO *pFrameInfo);
	COM_METHOD(HRESULT) EnumModules(ICorProfilerModuleEnum **ppEnum);
	COM_METHOD(HRESULT) GetRuntimeInformation(USHORT *pClrInstanceId, COR_PRF_RUNTIME_TYPE *pRuntimeType, USHORT *pMajorVersion, USHORT *pMinorVersion, USHORT *pBuildNumber, USHORT *pQFEVersion, ULONG cchVersionString, ULONG *pcchVersionString, _Out_writes_to_(cchVersionString, *pcchVersionString)  WCHAR szVersionString[  ]);
	COM_METHOD(HRESULT) GetThreadStaticAddress2(ClassID classId, mdFieldDef fieldToken, AppDomainID appDomainId, ThreadID threadId, void **ppAddress);
	COM_METHOD(HRESULT) GetAppDomainsContainingModule(ModuleID moduleId, ULONG32 cAppDomainIds, ULONG32 *pcAppDomainIds, AppDomainID appDomainIds[  ]);
	COM_METHOD(HRESULT) GetModuleInfo2(ModuleID moduleId, LPCBYTE *ppBaseLoadAddress, ULONG cchName, ULONG *pcchName, _Out_writes_to_(cchName, *pcchName)  WCHAR szName[  ], AssemblyID *pAssemblyId, DWORD *pdwModuleFlags);

private:
	ICorProfilerInfo3 *m_wrappedInfo;
	FILE *m_stream;
	volatile LONG m_refCount;
};
