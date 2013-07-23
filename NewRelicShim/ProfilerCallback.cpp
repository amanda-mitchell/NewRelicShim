#undef _WIN32_WINNT
#define _WIN32_WINNT	0x0403

#include <windows.h>
#include <share.h>

#include "basehlp.h"
#include "basehlp.hpp"

#include "ProfilerCallback.h"
#include "ProfilerInfoShim.h"

ProfilerCallback::ProfilerCallback() :
	m_refCount(0),
	m_stream(NULL),
	m_shimmedCallback(NULL),
	m_profilerInfo(NULL)
{
	HRESULT hr = S_OK;
	
	DWORD dwProcessId = GetCurrentProcessId();
	char fileName[256];
	sprintf_s(fileName, 256, "c:\\%d.log", dwProcessId);
	m_stream = _fsopen(fileName, "w+", _SH_DENYWR);

	IID newRelic = { 0xFF68FEB9, 0xE58A, 0x4B75, { 0xA2, 0xB8, 0x90, 0xCE, 0x7D, 0x91, 0x5A, 0x26 } };
	CoCreateInstance(newRelic, NULL, CLSCTX_INPROC_SERVER, IID_ICorProfilerCallback3, (LPVOID *) &m_shimmedCallback);
}

ProfilerCallback::~ProfilerCallback()
{
	if (m_stream != NULL)
	{
		fprintf(m_stream, "Destroying callback.\n");
		fclose(m_stream);
		m_stream = NULL;
	}

	if (m_profilerInfo != NULL)
	{
		m_profilerInfo->Release();
		m_profilerInfo = NULL;
	}

	if (m_shimmedCallback != NULL)
	{
		m_shimmedCallback->Release();
		m_shimmedCallback = NULL;
	}
}

ULONG ProfilerCallback::AddRef() 
{
	return InterlockedIncrement(&m_refCount);
}

ULONG ProfilerCallback::Release() 
{
	ULONG refCount = InterlockedDecrement(&m_refCount);
	if (refCount == 0)
		delete this;

	return refCount;
}

HRESULT ProfilerCallback::QueryInterface(REFIID riid, void **ppInterface)
{
	if (riid == IID_IUnknown)
		*ppInterface = static_cast<IUnknown *>(this); 
	else if (riid == IID_ICorProfilerCallback)
		*ppInterface = static_cast<ICorProfilerCallback *>(this);
	else if (riid == IID_ICorProfilerCallback2)
		*ppInterface = static_cast<ICorProfilerCallback2 *>(this);
	else if (riid == IID_ICorProfilerCallback3)
		*ppInterface = static_cast<ICorProfilerCallback3 *>(this);
	else
		*ppInterface = NULL;

	if (*ppInterface == NULL)
		return E_NOINTERFACE;

	reinterpret_cast<IUnknown *>(*ppInterface)->AddRef();

	return S_OK;
}

HRESULT ProfilerCallback::CreateObject(REFIID riid, void **ppInterface)
{
	HRESULT hr = E_NOINTERFACE;

	*ppInterface = NULL;
	if ((riid == IID_IUnknown)
		|| (riid == IID_ICorProfilerCallback3)
		|| (riid == IID_ICorProfilerCallback2) 
		|| (riid == IID_ICorProfilerCallback))
	{		   
		ProfilerCallback *pProfilerCallback = new ProfilerCallback();
		if (pProfilerCallback != NULL)
		{
			hr = S_OK;
			
			pProfilerCallback->AddRef();
			*ppInterface = static_cast<ICorProfilerCallback *>(pProfilerCallback);
		}
		else
		{
			hr = E_OUTOFMEMORY;
		}
	}	

	return hr;
}

HRESULT ProfilerCallback::Initialize(IUnknown *pICorProfilerInfoUnk)
{	
	if (pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo3, (LPVOID *) &m_profilerInfo) != S_OK || m_profilerInfo == NULL)
		return E_FAIL;

	ProfilerInfoShim *infoShim = new ProfilerInfoShim(m_profilerInfo, m_stream);
	if (infoShim == NULL)
		return E_FAIL;

	m_shimmedCallback->Initialize(infoShim);

	return S_OK;
}

HRESULT ProfilerCallback::Shutdown()
{
	fprintf(m_stream, "Profiler Shutdown\n");
	fflush(m_stream);
	return m_shimmedCallback->Shutdown();
}

HRESULT ProfilerCallback::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	HRESULT result = m_shimmedCallback->ModuleLoadFinished(moduleID, hrStatus);
	if (result != S_OK)
		return result;

	return result;
}

HRESULT ProfilerCallback::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
	return S_OK;
}

HRESULT ProfilerCallback::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL *pbUseCachedFunction)
{
	fprintf(m_stream, "Profiler JITCachedFunctionSearchStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->JITCachedFunctionSearchStarted(functionID, pbUseCachedFunction);
}

HRESULT ProfilerCallback::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
	return S_OK;
}

HRESULT ProfilerCallback::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result)
{
	fprintf(m_stream, "Profiler JITCachedFunctionSearchFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->JITCachedFunctionSearchFinished(functionID, result);
}

HRESULT ProfilerCallback::ExceptionUnwindFunctionEnter(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionUnwindFunctionEnter\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionUnwindFunctionEnter(functionID);
}

HRESULT ProfilerCallback::ExceptionUnwindFunctionLeave()
{
	fprintf(m_stream, "Profiler ExceptionUnwindFunctionLeave\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionUnwindFunctionLeave();
}

HRESULT ProfilerCallback::ThreadCreated(ThreadID threadID)
{
	fprintf(m_stream, "Profiler ThreadCreated\n");
	fflush(m_stream);
	return m_shimmedCallback->ThreadCreated(threadID);
}

HRESULT ProfilerCallback::ThreadDestroyed(ThreadID threadID)
{
	fprintf(m_stream, "Profiler ThreadDestroyed\n");
	fflush(m_stream);
	return m_shimmedCallback->ThreadDestroyed(threadID);
}

HRESULT ProfilerCallback::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID) 
{
	fprintf(m_stream, "Profiler ThreadAssignedToOSThread\n");
	fflush(m_stream);
	return m_shimmedCallback->ThreadAssignedToOSThread(managedThreadID, osThreadID);
}

HRESULT ProfilerCallback::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	fprintf(m_stream, "Profiler UnmanagedToManagedTransition\n");
	fflush(m_stream);
	return m_shimmedCallback->UnmanagedToManagedTransition(functionID, reason);
}

HRESULT ProfilerCallback::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	fprintf(m_stream, "Profiler ManagedToUnmanagedTransition\n");
	fflush(m_stream);
	return m_shimmedCallback->ManagedToUnmanagedTransition(functionID, reason);
}

HRESULT ProfilerCallback::ObjectAllocated(ObjectID objectID, ClassID classID)
{
	fprintf(m_stream, "Profiler ObjectAllocated\n");
	fflush(m_stream);
	return m_shimmedCallback->ObjectAllocated(objectID, classID);
}

HRESULT ProfilerCallback::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[])
{
	fprintf(m_stream, "Profiler ObjectReferences\n");
	fflush(m_stream);
	return m_shimmedCallback->ObjectReferences(objectID, classID, objectRefs, objectRefIDs);
}

HRESULT ProfilerCallback::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[])
{
	fprintf(m_stream, "Profiler RootReferences\n");
	fflush(m_stream);
	return m_shimmedCallback->RootReferences(rootRefs, rootRefIDs);
}

HRESULT ProfilerCallback::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
	fprintf(m_stream, "Profiler RuntimeSuspendStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeSuspendStarted(suspendReason);
}

HRESULT ProfilerCallback::RuntimeResumeFinished()
{
	fprintf(m_stream, "Profiler RuntimeResumeFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeResumeFinished();
}

HRESULT ProfilerCallback::AppDomainCreationStarted(AppDomainID appDomainID)
{
	fprintf(m_stream, "Profiler AppDomainCreationStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->AppDomainCreationStarted(appDomainID);
}

HRESULT ProfilerCallback::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler AppDomainCreationFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->AppDomainCreationFinished(appDomainID, hrStatus);
}

HRESULT ProfilerCallback::AppDomainShutdownStarted(AppDomainID appDomainID)
{
	fprintf(m_stream, "Profiler AppDomainShutdownStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->AppDomainShutdownStarted(appDomainID);
}

HRESULT ProfilerCallback::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler AppDomainShutdownFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->AppDomainShutdownFinished(appDomainID, hrStatus);
}

HRESULT ProfilerCallback::AssemblyLoadStarted(AssemblyID assemblyId)
{
	fprintf(m_stream, "Profiler AssemblyLoadStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->AssemblyLoadStarted(assemblyId);
}

HRESULT ProfilerCallback::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler AssemblyLoadFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->AssemblyLoadFinished(assemblyId, hrStatus);
}

HRESULT ProfilerCallback::AssemblyUnloadStarted(AssemblyID assemblyID)
{
	fprintf(m_stream, "Profiler AssemblyUnloadStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->AssemblyUnloadStarted(assemblyID);
}

HRESULT ProfilerCallback::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler AssemblyUnloadFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->AssemblyUnloadFinished(assemblyID, hrStatus);
}

HRESULT ProfilerCallback::ModuleLoadStarted(ModuleID moduleID)
{
	return m_shimmedCallback->ModuleLoadStarted(moduleID);
}

HRESULT ProfilerCallback::ModuleUnloadStarted(ModuleID moduleID)
{
	fprintf(m_stream, "Profiler ModuleUnloadStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->ModuleUnloadStarted(moduleID);
}

HRESULT ProfilerCallback::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler ModuleUnloadFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->ModuleUnloadFinished(moduleID, hrStatus);
}

HRESULT ProfilerCallback::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID)
{
	HRESULT result = m_shimmedCallback->ModuleAttachedToAssembly(moduleID, assemblyID);
	if (result != S_OK)
		return result;

	WCHAR moduleName[256];
	if (m_profilerInfo->GetModuleInfo(moduleID, NULL, 256, NULL, moduleName, NULL) != S_OK)
		return result;

	if (lstrcmpW(moduleName, L"C:\\Windows\\Microsoft.Net\\assembly\\GAC_MSIL\\System.ServiceModel\\v4.0_4.0.0.0__b77a5c561934e089\\System.ServiceModel.dll") == 0 ||
		lstrcmpW(moduleName, L"C:\\Windows\\Microsoft.Net\\assembly\\GAC_MSIL\\System.Web.Mvc\\v4.0_3.0.0.0__31bf3856ad364e35\\System.Web.Mvc.dll") == 0 ||
		lstrcmpW(moduleName, L"C:\\Windows\\Microsoft.Net\\assembly\\GAC_64\\System.Web\\v4.0_4.0.0.0__b03f5f7f11d50a3a\\System.Web.dll") == 0 ||
		lstrcmpW(moduleName, L"C:\\Windows\\Microsoft.Net\\assembly\\GAC_64\\System.Data\\v4.0_4.0.0.0__b77a5c561934e089\\System.Data.dll") == 0 ||
		lstrcmpW(moduleName, L"C:\\Windows\\Microsoft.Net\\assembly\\GAC_MSIL\\System\\v4.0_4.0.0.0__b77a5c561934e089\\System.dll") == 0 ||
		lstrcmpW(moduleName, L"C:\\Windows\\Microsoft.Net\\assembly\\GAC_MSIL\\System.Web.Extensions\\v4.0_4.0.0.0__31bf3856ad364e35\\System.Web.Extensions.dll") == 0)
	{
		IMetaDataImport2 *import = NULL;
		if (m_profilerInfo->GetModuleMetaData(moduleID, ofRead, IID_IMetaDataImport2, (IUnknown **) &import) != S_OK || import == NULL)
			return result;

		HCORENUM typeEnum = NULL;
		mdTypeDef typeDefs[100];
		ULONG returnedTypeDefs = 0;
		HRESULT enumResult = 0;
		while ((enumResult = import->EnumTypeDefs(&typeEnum, typeDefs, 100, &returnedTypeDefs)) == S_OK && returnedTypeDefs != 0)
		{
			for (ULONG typeDef = 0; typeDef < returnedTypeDefs; typeDef++)
			{
				ClassID classId = NULL;
				if (m_profilerInfo->GetClassFromTokenAndTypeArgs(moduleID, typeDefs[typeDef], 0, NULL, &classId) != S_OK || classId == NULL)
					continue;

				HCORENUM methodEnum = NULL;
				mdMethodDef methodDefs[100];
				ULONG returnedMethodDefs = 0;
				while (import->EnumMethods(&methodEnum, typeDefs[typeDef], methodDefs, 100, &returnedMethodDefs) == S_OK && returnedMethodDefs != 0)
				{
					for (ULONG methodDef = 0; methodDef < returnedMethodDefs; methodDef++)
					{
						FunctionID function = NULL;
						if ( m_profilerInfo->GetFunctionFromTokenAndTypeArgs(moduleID, methodDefs[methodDef], classId, 0, NULL, &function) == S_OK && function != NULL)
						{
							m_shimmedCallback->JITCompilationStarted(function, TRUE);
							m_shimmedCallback->JITCompilationFinished(function, S_OK, TRUE);
						}
					}
				}
			}
		}
	}

	return result;
}

HRESULT ProfilerCallback::ClassLoadStarted(ClassID classID)
{
	fprintf(m_stream, "Profiler ClassLoadStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->ClassLoadStarted(classID);
}

HRESULT ProfilerCallback::ClassLoadFinished(ClassID classID, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler ClassLoadFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->ClassLoadFinished(classID, hrStatus);
}

HRESULT ProfilerCallback::ClassUnloadStarted(ClassID classID)
{
	fprintf(m_stream, "Profiler ClassUnloadStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->ClassUnloadStarted(classID);
}

HRESULT ProfilerCallback::ClassUnloadFinished(ClassID classID, HRESULT hrStatus)
{
	fprintf(m_stream, "Profiler ClassUnloadFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->ClassUnloadFinished(classID, hrStatus);
}

HRESULT ProfilerCallback::FunctionUnloadStarted(FunctionID functionID)
{
	fprintf(m_stream, "Profiler FunctionUnloadStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->FunctionUnloadStarted(functionID);
}

HRESULT ProfilerCallback::JITFunctionPitched(FunctionID functionID)
{
	fprintf(m_stream, "Profiler JITFunctionPitched\n");
	fflush(m_stream);
	return m_shimmedCallback->JITFunctionPitched(functionID);
}

HRESULT ProfilerCallback::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline)
{
	return S_OK;
}

HRESULT ProfilerCallback::RemotingClientInvocationStarted()
{
	fprintf(m_stream, "Profiler RemotingClientInvocationStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingClientInvocationStarted();
}

HRESULT ProfilerCallback::RemotingClientSendingMessage(GUID *pCookie, BOOL fIsAsync)
{
	fprintf(m_stream, "Profiler RemotingClientSendingMessage\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingClientSendingMessage(pCookie, fIsAsync);
}

HRESULT ProfilerCallback::RemotingClientReceivingReply(GUID *pCookie, BOOL fIsAsync)
{
	fprintf(m_stream, "Profiler RemotingClientReceivingReply\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingClientReceivingReply(pCookie, fIsAsync);
}

HRESULT ProfilerCallback::RemotingClientInvocationFinished()
{
	fprintf(m_stream, "Profiler RemotingClientInvocationFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingClientInvocationFinished();
}

HRESULT ProfilerCallback::RemotingServerReceivingMessage(GUID *pCookie, BOOL fIsAsync)
{
	fprintf(m_stream, "Profiler RemotingServerReceivingMessage\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingServerReceivingMessage(pCookie, fIsAsync);
}

HRESULT ProfilerCallback::RemotingServerInvocationStarted()
{
	fprintf(m_stream, "Profiler RemotingServerInvocationStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingServerInvocationStarted();
}

HRESULT ProfilerCallback::RemotingServerInvocationReturned()
{
	fprintf(m_stream, "Profiler RemotingServerInvocationReturned\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingServerInvocationReturned();
}

HRESULT ProfilerCallback::RemotingServerSendingReply(GUID *pCookie, BOOL fIsAsync)
{
	fprintf(m_stream, "Profiler RemotingServerSendingReply\n");
	fflush(m_stream);
	return m_shimmedCallback->RemotingServerSendingReply(pCookie, fIsAsync);
}

HRESULT ProfilerCallback::RuntimeSuspendFinished()
{
	fprintf(m_stream, "Profiler RuntimeSuspendFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeSuspendFinished();
}

HRESULT ProfilerCallback::RuntimeSuspendAborted()
{
	fprintf(m_stream, "Profiler RuntimeSuspendAborted\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeSuspendAborted();
}

HRESULT ProfilerCallback::RuntimeResumeStarted()
{
	fprintf(m_stream, "Profiler RuntimeResumeStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeResumeStarted();
}

HRESULT ProfilerCallback::RuntimeThreadSuspended(ThreadID threadID)
{
	fprintf(m_stream, "Profiler RuntimeThreadSuspended\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeThreadSuspended(threadID);
}

HRESULT ProfilerCallback::RuntimeThreadResumed(ThreadID threadID)
{
	fprintf(m_stream, "Profiler RuntimeThreadResumed\n");
	fflush(m_stream);
	return m_shimmedCallback->RuntimeThreadResumed(threadID);
}

HRESULT ProfilerCallback::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	fprintf(m_stream, "Profiler MovedReferences\n");
	fflush(m_stream);
	return m_shimmedCallback->MovedReferences(cmovedObjectIDRanges, oldObjectIDRangeStart, newObjectIDRangeStart, cObjectIDRangeLength);
}

HRESULT ProfilerCallback::SurvivingReferences(ULONG cmovedObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	fprintf(m_stream, "Profiler SurvivingReferences\n");
	fflush(m_stream);
	return m_shimmedCallback->SurvivingReferences(cmovedObjectIDRanges, objectIDRangeStart, cObjectIDRangeLength);
}

HRESULT ProfilerCallback::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[])
{
	fprintf(m_stream, "Profiler ObjectsAllocatedByClass\n");
	fflush(m_stream);
	return m_shimmedCallback->ObjectsAllocatedByClass(classCount, classIDs, objects);
}

HRESULT ProfilerCallback::ExceptionThrown(ObjectID thrownObjectID)
{
	fprintf(m_stream, "Profiler ExceptionThrown\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionThrown(thrownObjectID);
}

HRESULT ProfilerCallback::ExceptionSearchFunctionEnter(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionSearchFunctionEnter\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionSearchFunctionEnter(functionID);
}

HRESULT ProfilerCallback::ExceptionSearchFunctionLeave()
{
	fprintf(m_stream, "Profiler ExceptionSearchFunctionLeave\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionSearchFunctionLeave();
}

HRESULT ProfilerCallback::ExceptionSearchFilterEnter(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionSearchFilterEnter\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionSearchFilterEnter(functionID);
}

HRESULT ProfilerCallback::ExceptionSearchFilterLeave()
{
	fprintf(m_stream, "Profiler ExceptionSearchFilterLeave\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionSearchFilterLeave();
}

HRESULT ProfilerCallback::ExceptionSearchCatcherFound(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionSearchCatcherFound\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionSearchCatcherFound(functionID);
}

HRESULT ProfilerCallback::ExceptionCLRCatcherFound()
{
	fprintf(m_stream, "Profiler ExceptionCLRCatcherFound\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionCLRCatcherFound();
}

HRESULT ProfilerCallback::ExceptionCLRCatcherExecute()
{
	fprintf(m_stream, "Profiler ExceptionCLRCatcherExecute\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionCLRCatcherExecute();
}

HRESULT ProfilerCallback::ExceptionOSHandlerEnter(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionOSHandlerEnter\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionOSHandlerEnter(functionID);
}

HRESULT ProfilerCallback::ExceptionOSHandlerLeave(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionOSHandlerLeave\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionOSHandlerLeave(functionID);
}

HRESULT ProfilerCallback::ExceptionUnwindFinallyEnter(FunctionID functionID)
{
	fprintf(m_stream, "Profiler ExceptionUnwindFinallyEnter\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionUnwindFinallyEnter(functionID);
}

HRESULT ProfilerCallback::ExceptionUnwindFinallyLeave()
{
	fprintf(m_stream, "Profiler ExceptionUnwindFinallyLeave\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionUnwindFinallyLeave();
}

HRESULT ProfilerCallback::ExceptionCatcherEnter(FunctionID functionID, ObjectID objectID)
{
	fprintf(m_stream, "Profiler ExceptionCatcherEnter\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionCatcherEnter(functionID, objectID);
}

HRESULT ProfilerCallback::ExceptionCatcherLeave()
{
	fprintf(m_stream, "Profiler ExceptionCatcherLeave\n");
	fflush(m_stream);
	return m_shimmedCallback->ExceptionCatcherLeave();
}

HRESULT ProfilerCallback::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots)
{
	fprintf(m_stream, "Profiler COMClassicVTableCreated\n");
	fflush(m_stream);
	return m_shimmedCallback->COMClassicVTableCreated(wrappedClassID, implementedIID, pVTable, cSlots);
}

HRESULT ProfilerCallback::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable)
{
	fprintf(m_stream, "Profiler COMClassicVTableDestroyed\n");
	fflush(m_stream);
	return m_shimmedCallback->COMClassicVTableDestroyed(wrappedClassID, implementedIID, pVTable);
}

HRESULT ProfilerCallback::ThreadNameChanged(ThreadID threadId, ULONG cchName, WCHAR name[  ])
{
	fprintf(m_stream, "Profiler ThreadNameChanged\n");
	fflush(m_stream);
	return m_shimmedCallback->ThreadNameChanged(threadId, cchName, name);
}

HRESULT ProfilerCallback::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
	fprintf(m_stream, "Profiler FinalizeableObjectQueued\n");
	fflush(m_stream);
	return m_shimmedCallback->FinalizeableObjectQueued(finalizerFlags, objectID);
}

HRESULT ProfilerCallback::RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[  ], COR_PRF_GC_ROOT_KIND rootKinds[  ], COR_PRF_GC_ROOT_FLAGS rootFlags[  ], UINT_PTR rootIds[  ])
{
	fprintf(m_stream, "Profiler RootReferences2\n");
	fflush(m_stream);
	return m_shimmedCallback->RootReferences2(cRootRefs, rootRefIds, rootKinds, rootFlags, rootIds);
}

HRESULT ProfilerCallback::HandleCreated(UINT_PTR handleId, ObjectID initialObjectId)
{
	fprintf(m_stream, "Profiler HandleCreated\n");
	fflush(m_stream);
	return m_shimmedCallback->HandleCreated(handleId, initialObjectId);
}

HRESULT ProfilerCallback::HandleDestroyed(UINT_PTR handleId)
{
	fprintf(m_stream, "Profiler HandleDestroyed\n");
	fflush(m_stream);
	return m_shimmedCallback->HandleDestroyed(handleId);
}

HRESULT ProfilerCallback::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
	fprintf(m_stream, "Profiler GarbageCollectionStarted\n");
	fflush(m_stream);
	return m_shimmedCallback->GarbageCollectionStarted(cGenerations, generationCollected, reason);
}

HRESULT  ProfilerCallback::GarbageCollectionFinished()
{
	fprintf(m_stream, "Profiler GarbageCollectionFinished\n");
	fflush(m_stream);
	return m_shimmedCallback->GarbageCollectionFinished();
}

HRESULT ProfilerCallback::InitializeForAttach(IUnknown *pCorProfilerInfoUnk, void *pvClientData, UINT cbClientData)
{
	fprintf(m_stream, "Profiler InitializeForAttach\n");
	fflush(m_stream);
	return m_shimmedCallback->InitializeForAttach(pCorProfilerInfoUnk, pvClientData, cbClientData);
}

HRESULT ProfilerCallback::ProfilerAttachComplete(void)
{
	fprintf(m_stream, "Profiler ProfilerAttachComplete\n");
	fflush(m_stream);
	return m_shimmedCallback->ProfilerAttachComplete();
}
		
HRESULT ProfilerCallback::ProfilerDetachSucceeded(void)
{
	fprintf(m_stream, "Profiler ProfilerDetachSucceeded\n");
	fflush(m_stream);
	return m_shimmedCallback->ProfilerDetachSucceeded();
}

#include "dllmain.hpp"
