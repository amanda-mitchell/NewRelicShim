#ifndef __PROFILER_CALLBACK_H__
#define __PROFILER_CALLBACK_H__

#include "mscoree.h"
#include "basehlp.h"


extern const GUID __declspec( selectany ) CLSID_PROFILER = 
	{ 0x39aa112, 0x7d12, 0x4089, { 0xab, 0x1c, 0x1, 0xc4, 0xcb, 0xea, 0xf2, 0x37} };

#define THREADING_MODEL "Both"
#define PROGID_PREFIX "Profiler Shim"
#define COCLASS_DESCRIPTION "CLR Profiler Shim"
#define PROFILER_GUID "{039AA112-7D12-4089-AB1C-01C4CBEAF237}"

class ProfilerCallback :
    public ICorProfilerCallback3
{
    public:
    
        ProfilerCallback();
        ~ProfilerCallback();

    public:

        COM_METHOD( ULONG ) AddRef(); 
        COM_METHOD( ULONG ) Release();
        COM_METHOD( HRESULT ) QueryInterface( REFIID riid, void **ppInterface );

        COM_METHOD( HRESULT ) Initialize( IUnknown *pICorProfilerInfoUnk );
        COM_METHOD( HRESULT ) Shutdown();                                         
        COM_METHOD( HRESULT ) AppDomainCreationStarted( AppDomainID appDomainID );
        COM_METHOD( HRESULT ) AppDomainCreationFinished( AppDomainID appDomainID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) AppDomainShutdownStarted( AppDomainID appDomainID );
        COM_METHOD( HRESULT ) AppDomainShutdownFinished( AppDomainID appDomainID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) AssemblyLoadStarted( AssemblyID assemblyID );
        COM_METHOD( HRESULT ) AssemblyLoadFinished( AssemblyID assemblyID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) AssemblyUnloadStarted( AssemblyID assemblyID );
        COM_METHOD( HRESULT ) AssemblyUnloadFinished( AssemblyID assemblyID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) ModuleLoadStarted( ModuleID moduleID );
        COM_METHOD( HRESULT ) ModuleLoadFinished( ModuleID moduleID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) ModuleUnloadStarted( ModuleID moduleID );
        COM_METHOD( HRESULT ) ModuleUnloadFinished( ModuleID moduleID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) ModuleAttachedToAssembly( ModuleID moduleID, AssemblyID assemblyID );
        COM_METHOD( HRESULT ) ClassLoadStarted( ClassID classID );
        COM_METHOD( HRESULT ) ClassLoadFinished( ClassID classID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) ClassUnloadStarted( ClassID classID );
        COM_METHOD( HRESULT ) ClassUnloadFinished( ClassID classID, HRESULT hrStatus );
        COM_METHOD( HRESULT ) FunctionUnloadStarted( FunctionID functionID );
        COM_METHOD( HRESULT ) JITCompilationStarted( FunctionID functionID, BOOL fIsSafeToBlock );
        COM_METHOD( HRESULT ) JITCompilationFinished( FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock );
        COM_METHOD( HRESULT ) JITCachedFunctionSearchStarted( FunctionID functionID, BOOL *pbUseCachedFunction );
        COM_METHOD( HRESULT ) JITCachedFunctionSearchFinished( FunctionID functionID, COR_PRF_JIT_CACHE result );
        COM_METHOD( HRESULT ) JITFunctionPitched( FunctionID functionID );
        COM_METHOD( HRESULT ) JITInlining( FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline );
        COM_METHOD( HRESULT ) ThreadCreated( ThreadID threadID );
        COM_METHOD( HRESULT ) ThreadDestroyed( ThreadID threadID );
        COM_METHOD( HRESULT ) ThreadAssignedToOSThread( ThreadID managedThreadID, DWORD osThreadID );
        COM_METHOD( HRESULT ) RemotingClientInvocationStarted();
        COM_METHOD( HRESULT ) RemotingClientSendingMessage( GUID *pCookie, BOOL fIsAsync );
        COM_METHOD( HRESULT ) RemotingClientReceivingReply( GUID *pCookie, BOOL fIsAsync );
        COM_METHOD( HRESULT ) RemotingClientInvocationFinished();
        COM_METHOD( HRESULT ) RemotingServerReceivingMessage( GUID *pCookie, BOOL fIsAsync );
        COM_METHOD( HRESULT ) RemotingServerInvocationStarted();
        COM_METHOD( HRESULT ) RemotingServerInvocationReturned();
        COM_METHOD( HRESULT ) RemotingServerSendingReply( GUID *pCookie, BOOL fIsAsync );
        COM_METHOD( HRESULT ) UnmanagedToManagedTransition( FunctionID functionID, COR_PRF_TRANSITION_REASON reason );
        COM_METHOD( HRESULT ) ManagedToUnmanagedTransition( FunctionID functionID, COR_PRF_TRANSITION_REASON reason );
        COM_METHOD( HRESULT ) RuntimeSuspendStarted( COR_PRF_SUSPEND_REASON suspendReason );
        COM_METHOD( HRESULT ) RuntimeSuspendFinished();
        COM_METHOD( HRESULT ) RuntimeSuspendAborted();
        COM_METHOD( HRESULT ) RuntimeResumeStarted();
        COM_METHOD( HRESULT ) RuntimeResumeFinished();
        COM_METHOD( HRESULT ) RuntimeThreadSuspended( ThreadID threadid );
        COM_METHOD( HRESULT ) RuntimeThreadResumed( ThreadID threadid );
        COM_METHOD( HRESULT ) MovedReferences( ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[] );
        COM_METHOD( HRESULT ) SurvivingReferences( ULONG cmovedObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[] );
        COM_METHOD( HRESULT ) ObjectAllocated( ObjectID objectID, ClassID classID );
        COM_METHOD( HRESULT ) ObjectsAllocatedByClass( ULONG classCount, ClassID classIDs[], ULONG objects[] );
        COM_METHOD( HRESULT ) ObjectReferences( ObjectID objectID, ClassID classID, ULONG cObjectRefs, ObjectID objectRefIDs[] );
        COM_METHOD( HRESULT ) RootReferences( ULONG cRootRefs, ObjectID rootRefIDs[] );
        COM_METHOD( HRESULT ) ExceptionThrown( ObjectID thrownObjectID );
        COM_METHOD( HRESULT ) ExceptionSearchFunctionEnter( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionSearchFunctionLeave();
        COM_METHOD( HRESULT ) ExceptionSearchFilterEnter( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionSearchFilterLeave();
        COM_METHOD( HRESULT ) ExceptionSearchCatcherFound( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionCLRCatcherFound();
        COM_METHOD( HRESULT ) ExceptionCLRCatcherExecute();
        COM_METHOD( HRESULT ) ExceptionOSHandlerEnter( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionOSHandlerLeave( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionUnwindFunctionEnter( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionUnwindFunctionLeave();
        COM_METHOD( HRESULT ) ExceptionUnwindFinallyEnter( FunctionID functionID );
        COM_METHOD( HRESULT ) ExceptionUnwindFinallyLeave();
        COM_METHOD( HRESULT ) ExceptionCatcherEnter( FunctionID functionID, ObjectID objectID );    
        COM_METHOD( HRESULT ) ExceptionCatcherLeave();
        COM_METHOD( HRESULT )  COMClassicVTableCreated( ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots );
        COM_METHOD( HRESULT )  COMClassicVTableDestroyed( ClassID wrappedClassID, REFGUID implementedIID, void *pVTable );
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE ThreadNameChanged( ThreadID threadId, ULONG cchName, WCHAR name[  ]);
        COM_METHOD( HRESULT )  GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason);
        COM_METHOD( HRESULT )  GarbageCollectionFinished();
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE RootReferences2( ULONG cRootRefs, ObjectID rootRefIds[  ], COR_PRF_GC_ROOT_KIND rootKinds[  ], COR_PRF_GC_ROOT_FLAGS rootFlags[  ], UINT_PTR rootIds[  ]);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE HandleCreated( UINT_PTR handleId, ObjectID initialObjectId);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE HandleDestroyed(UINT_PTR handleId);
		COM_METHOD( HRESULT ) STDMETHODCALLTYPE InitializeForAttach( IUnknown *pCorProfilerInfoUnk, void *pvClientData, UINT cbClientData);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE ProfilerAttachComplete( void);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE ProfilerDetachSucceeded( void);
		COM_METHOD( HRESULT ) STDMETHODCALLTYPE ReJITCompilationStarted( FunctionID functionId, ReJITID rejitId, BOOL fIsSafeToBlock);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE GetReJITParameters( ModuleID moduleId, mdMethodDef methodId, ICorProfilerFunctionControl *pFunctionControl);
		COM_METHOD( HRESULT ) STDMETHODCALLTYPE ReJITCompilationFinished( FunctionID functionId, ReJITID rejitId, HRESULT hrStatus, BOOL fIsSafeToBlock);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE ReJITError( ModuleID moduleId, mdMethodDef methodId, FunctionID functionId, HRESULT hrStatus);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE MovedReferences2( ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[  ], ObjectID newObjectIDRangeStart[  ], SIZE_T cObjectIDRangeLength[  ]);
        COM_METHOD( HRESULT ) STDMETHODCALLTYPE SurvivingReferences2( ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[  ], SIZE_T cObjectIDRangeLength[  ]);
		COM_METHOD( HRESULT ) STDMETHODCALLTYPE ConditionalWeakTableElementReferences( ULONG cRootRefs, ObjectID keyRefIds[  ], ObjectID valueRefIds[  ], GCHandleID rootIds[  ]);
        static COM_METHOD( HRESULT) CreateObject( REFIID riid, void **ppInterface );            

	private:
        
        volatile LONG m_refCount;
        FILE *m_stream;
		ICorProfilerCallback3 *m_shimmedCallback;
		ICorProfilerInfo3 *m_profilerInfo;
};

#endif
