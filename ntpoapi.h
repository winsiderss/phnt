/*
 * This file is part of the Process Hacker project - https://processhacker.sourceforge.io/
 *
 * You can redistribute this file and/or modify it under the terms of the 
 * Attribution 4.0 International (CC BY 4.0) license. 
 * 
 * You must give appropriate credit, provide a link to the license, and 
 * indicate if changes were made. You may do so in any reasonable manner, but 
 * not in any way that suggests the licensor endorses you or your use.
 */

#ifndef _NTPOAPI_H
#define _NTPOAPI_H

typedef union _POWER_STATE
{
    SYSTEM_POWER_STATE SystemState;
    DEVICE_POWER_STATE DeviceState;
} POWER_STATE, *PPOWER_STATE;

typedef enum _POWER_STATE_TYPE
{
    SystemPowerState = 0,
    DevicePowerState
} POWER_STATE_TYPE, *PPOWER_STATE_TYPE;

#if (PHNT_VERSION >= PHNT_VISTA)
// wdm
typedef struct _SYSTEM_POWER_STATE_CONTEXT
{
    union
    {
        struct
        {
            ULONG Reserved1 : 8;
            ULONG TargetSystemState : 4;
            ULONG EffectiveSystemState : 4;
            ULONG CurrentSystemState : 4;
            ULONG IgnoreHibernationPath : 1;
            ULONG PseudoTransition : 1;
            ULONG Reserved2 : 10;
        };
        ULONG ContextAsUlong;
    };
} SYSTEM_POWER_STATE_CONTEXT, *PSYSTEM_POWER_STATE_CONTEXT;
#endif

#if (PHNT_VERSION >= PHNT_WIN7)
#define  POWER_REQUEST_CONTEXT_NOT_SPECIFIED DIAGNOSTIC_REASON_NOT_SPECIFIED

/** \cond NEVER */ // disable doxygen warning
// wdm
typedef struct _COUNTED_REASON_CONTEXT
{
    ULONG Version;
    ULONG Flags;
    union
    {
        struct
        {
            UNICODE_STRING ResourceFileName;
            USHORT ResourceReasonId;
            ULONG StringCount;
            PUNICODE_STRING _Field_size_(StringCount) ReasonStrings;
        };
        UNICODE_STRING SimpleString;
    };
} COUNTED_REASON_CONTEXT, *PCOUNTED_REASON_CONTEXT;
/** \endcond */
#endif

typedef enum _POWER_STATE_HANDLER_TYPE
{
    PowerStateSleeping1 = 0,
    PowerStateSleeping2 = 1,
    PowerStateSleeping3 = 2,
    PowerStateSleeping4 = 3,
    PowerStateShutdownOff = 4,
    PowerStateShutdownReset = 5,
    PowerStateSleeping4Firmware = 6,
    PowerStateMaximum = 7
} POWER_STATE_HANDLER_TYPE, *PPOWER_STATE_HANDLER_TYPE;

typedef NTSTATUS (NTAPI *PENTER_STATE_SYSTEM_HANDLER)(
    _In_ PVOID SystemContext
    );

typedef NTSTATUS (NTAPI *PENTER_STATE_HANDLER)(
    _In_ PVOID Context,
    _In_opt_ PENTER_STATE_SYSTEM_HANDLER SystemHandler,
    _In_ PVOID SystemContext,
    _In_ LONG NumberProcessors,
    _In_ LONG volatile *Number
    );

typedef struct _POWER_STATE_HANDLER
{
    POWER_STATE_HANDLER_TYPE Type;
    BOOLEAN RtcWake;
    UCHAR Spare[3];
    PENTER_STATE_HANDLER Handler;
    PVOID Context;
} POWER_STATE_HANDLER, *PPOWER_STATE_HANDLER;

typedef NTSTATUS (NTAPI *PENTER_STATE_NOTIFY_HANDLER)(
    _In_ POWER_STATE_HANDLER_TYPE State,
    _In_ PVOID Context,
    _In_ BOOLEAN Entering
    );

typedef struct _POWER_STATE_NOTIFY_HANDLER
{
    PENTER_STATE_NOTIFY_HANDLER Handler;
    PVOID Context;
} POWER_STATE_NOTIFY_HANDLER, *PPOWER_STATE_NOTIFY_HANDLER;

#if (PHNT_MODE != PHNT_MODE_KERNEL)
// POWER_INFORMATION_LEVEL
// Note: We don't use an enum for these values to minimize conflicts with the Windows SDK. (dmex)
#define SystemPowerPolicyAc 0 // SYSTEM_POWER_POLICY // GET: InputBuffer NULL. SET: InputBuffer not NULL.
#define SystemPowerPolicyDc 1 // SYSTEM_POWER_POLICY
#define VerifySystemPolicyAc 2 // SYSTEM_POWER_POLICY
#define VerifySystemPolicyDc 3 // SYSTEM_POWER_POLICY
#define SystemPowerCapabilities 4 // SYSTEM_POWER_CAPABILITIES
#define SystemBatteryState 5 // SYSTEM_BATTERY_STATE
#define SystemPowerStateHandler 6 // (kernel-mode only)
#define ProcessorStateHandler 7 // (kernel-mode only)
#define SystemPowerPolicyCurrent 8 // SYSTEM_POWER_POLICY
#define AdministratorPowerPolicy 9 // ADMINISTRATOR_POWER_POLICY
#define SystemReserveHiberFile 10 // BOOLEAN // (requires SeCreatePagefilePrivilege) // TRUE: hibernation file created. FALSE: hibernation file deleted.
#define ProcessorInformation 11 // PROCESSOR_POWER_INFORMATION
#define SystemPowerInformation 12 // SYSTEM_POWER_INFORMATION
#define ProcessorStateHandler2 13 // not implemented
#define LastWakeTime 14 // ULONGLONG
#define LastSleepTime 15 // ULONGLONG
#define SystemExecutionState 16 // EXECUTION_STATE // NtSetThreadExecutionState
#define SystemPowerStateNotifyHandler 17 // (kernel-mode only)
#define ProcessorPowerPolicyAc 18 // not implemented
#define ProcessorPowerPolicyDc 19 // not implemented
#define VerifyProcessorPowerPolicyAc 20 // not implemented
#define VerifyProcessorPowerPolicyDc 21 // not implemented
#define ProcessorPowerPolicyCurrent 22 // not implemented
#define SystemPowerStateLogging 23
#define SystemPowerLoggingEntry 24 // (kernel-mode only)
#define SetPowerSettingValue 25 // (kernel-mode only)
#define NotifyUserPowerSetting 26 // not implemented
#define PowerInformationLevelUnused0 27 // not implemented
#define SystemMonitorHiberBootPowerOff 28 // NULL (PowerMonitorOff)
#define SystemVideoState 29 // MONITOR_DISPLAY_STATE
#define TraceApplicationPowerMessage 30 // (kernel-mode only)
#define TraceApplicationPowerMessageEnd 31 // (kernel-mode only)
#define ProcessorPerfStates 32 // (kernel-mode only)
#define ProcessorIdleStates 33 // (kernel-mode only)
#define ProcessorCap 34 // (kernel-mode only)
#define SystemWakeSource 35
#define SystemHiberFileInformation 36
#define TraceServicePowerMessage 37
#define ProcessorLoad 38
#define PowerShutdownNotification 39 // (kernel-mode only)
#define MonitorCapabilities 40 // (kernel-mode only)
#define SessionPowerInit 41 // (kernel-mode only)
#define SessionDisplayState 42 // (kernel-mode only)
#define PowerRequestCreate 43 // in: COUNTED_REASON_CONTEXT, out: HANDLE
#define PowerRequestAction 44 // in: POWER_REQUEST_ACTION
#define GetPowerRequestList 45 // out: POWER_REQUEST_LIST
#define ProcessorInformationEx 46 // in: USHORT, out: PROCESSOR_POWER_INFORMATION
#define NotifyUserModeLegacyPowerEvent 47 // (kernel-mode only)
#define GroupPark 48 // (debug-mode boot only) 
#define ProcessorIdleDomains 49 // (kernel-mode only)
#define WakeTimerList 50 // powercfg.exe /waketimers
#define SystemHiberFileSize 51 // ULONG
#define ProcessorIdleStatesHv 52 // (kernel-mode only)
#define ProcessorPerfStatesHv 53 // (kernel-mode only)
#define ProcessorPerfCapHv 54 // (kernel-mode only)
#define ProcessorSetIdle 55 // (debug-mode boot only) 
#define LogicalProcessorIdling 56 // (kernel-mode only)
#define UserPresence 57 // not implemented
#define PowerSettingNotificationName 58
#define GetPowerSettingValue 59 // GUID
#define IdleResiliency 60 // POWER_IDLE_RESILIENCY
#define SessionRITState 61 // (kernel-mode only)
#define SessionConnectNotification 62 // (kernel-mode only)
#define SessionPowerCleanup 63 // (kernel-mode only)
#define SessionLockState 64
#define SystemHiberbootState 65 // BOOLEAN // fast startup supported
#define PlatformInformation 66 // BOOLEAN // connected standby supported
#define PdcInvocation 67 // (kernel-mode only)
#define MonitorInvocation 68 // (kernel-mode only)
#define FirmwareTableInformationRegistered 69 // (kernel-mode only)
#define SetShutdownSelectedTime 70 // NULL
#define SuspendResumeInvocation 71 // (kernel-mode only)
#define PlmPowerRequestCreate 72 // in: COUNTED_REASON_CONTEXT, out: HANDLE
#define ScreenOff 73 // NULL (PowerMonitorOff)
#define CsDeviceNotification 74 // (kernel-mode only)
#define PlatformRole 75 // POWER_PLATFORM_ROLE
#define LastResumePerformance 76 // RESUME_PERFORMANCE
#define DisplayBurst 77 // NULL (PowerMonitorOn)
#define ExitLatencySamplingPercentage 78
#define RegisterSpmPowerSettings 79 // (kernel-mode only)
#define PlatformIdleStates 80 // (kernel-mode only)
#define ProcessorIdleVeto 81 // (kernel-mode only) // deprecated
#define PlatformIdleVeto 82 // (kernel-mode only) // deprecated
#define SystemBatteryStatePrecise 83 // SYSTEM_BATTERY_STATE
#define ThermalEvent 84  // THERMAL_EVENT // PowerReportThermalEvent
#define PowerRequestActionInternal 85 // (kernel-mode only)
#define BatteryDeviceState 86
#define PowerInformationInternal 87
#define ThermalStandby 88 // NULL // shutdown with thermal standby as reason.
#define SystemHiberFileType 89 // ULONG // zero ? reduced : full // powercfg.exe /h /type 
#define PhysicalPowerButtonPress 90 // BOOLEAN
#define QueryPotentialDripsConstraint 91 // (kernel-mode only)
#define EnergyTrackerCreate 92
#define EnergyTrackerQuery 93
#define UpdateBlackBoxRecorder 94
#define SessionAllowExternalDmaDevices 95
#define PowerInformationLevelMaximum 96
#endif

#if (PHNT_VERSION >= PHNT_WIN7)
// POWER_REQUEST_TYPE
// Note: We don't use an enum since it conflicts with the Windows SDK.
#define PowerRequestDisplayRequired 0
#define PowerRequestSystemRequired 1
#define PowerRequestAwayModeRequired 2
#define PowerRequestExecutionRequired 3        // Windows 8+
#define PowerRequestPerfBoostRequired 4        // Windows 8+
#define PowerRequestActiveLockScreenRequired 5 // Windows 10 RS1+ (reserved on Windows 8)
// Values 6 and 7 are reserved for Windows 8 only
#define PowerRequestFullScreenVideoRequired 8  // Windows 8 only

typedef struct _POWER_REQUEST_ACTION
{
    HANDLE PowerRequest;
    POWER_REQUEST_TYPE RequestType;
    BOOLEAN Enable;
    HANDLE TargetProcess; // Windows 8+ and only for requests created via PlmPowerRequestCreate
} POWER_REQUEST_ACTION, * PPOWER_REQUEST_ACTION;

typedef struct _POWER_REQUEST_LIST
{
    ULONG_PTR cElements;
    ULONG_PTR OffsetsToRequests[ANYSIZE_ARRAY]; // PPOWER_REQUEST
} POWER_REQUEST_LIST, *PPOWER_REQUEST_LIST;

typedef enum _POWER_REQUEST_ORIGIN
{
    POWER_REQUEST_ORIGIN_DRIVER = 0,
    POWER_REQUEST_ORIGIN_PROCESS = 1,
    POWER_REQUEST_ORIGIN_SERVICE = 2
} POWER_REQUEST_ORIGIN;

typedef struct _POWER_REQUEST_BODY
{
    ULONG_PTR cbSize;
    POWER_REQUEST_ORIGIN Origin;
    ULONG_PTR OffsetToRequester; // PWSTR
    union
    {
        struct
        {
            ULONG ProcessId;
            ULONG ServiceTag;
        };
        ULONG_PTR OffsetToDriverName; // PWSTR
    };    
    ULONG_PTR OffsetToContext; // PCOUNTED_REASON_CONTEXT_RELATIVE
} POWER_REQUEST_BODY, *PPOWER_REQUEST_BODY;

// The number of supported request types per version
#define POWER_REQUEST_SUPPORTED_TYPES_V1 3 // Windows 7
#define POWER_REQUEST_SUPPORTED_TYPES_V2 9 // Windows 8
#define POWER_REQUEST_SUPPORTED_TYPES_V3 5 // Windows 8.1 and Windows 10 TH1-TH2
#define POWER_REQUEST_SUPPORTED_TYPES_V4 6 // Windows 10 RS1+

typedef struct _POWER_REQUEST
{
    union
    {
        struct
        {
            ULONG Reserved;
            ULONG ActiveCount[POWER_REQUEST_SUPPORTED_TYPES_V1];
            POWER_REQUEST_BODY Body;
        } V1;
#if (PHNT_VERSION >= PHNT_WIN8)
        struct
        {
            ULONG Reserved;
            ULONG ActiveCount[POWER_REQUEST_SUPPORTED_TYPES_V2];
            POWER_REQUEST_BODY Body;
        } V2;
#endif
#if (PHNT_VERSION >= PHNT_WINBLUE)
        struct
        {
            ULONG Reserved;
            ULONG ActiveCount[POWER_REQUEST_SUPPORTED_TYPES_V3];
            POWER_REQUEST_BODY Body;
        } V3;
#endif
#if (PHNT_VERSION >= PHNT_REDSTONE)
        struct
        {
            ULONG Reserved;
            ULONG ActiveCount[POWER_REQUEST_SUPPORTED_TYPES_V4];
            POWER_REQUEST_BODY Body;
        } V4;
#endif
    };
} POWER_REQUEST, *PPOWER_REQUEST;

typedef struct _COUNTED_REASON_CONTEXT_RELATIVE
{
    ULONG Flags;
    union
    {
        struct
        {
            ULONG_PTR OffsetToResourceFileName;
            USHORT ResourceReasonId;
            ULONG StringCount;
            ULONG_PTR OffsetToReasonStrings;
        };
        ULONG_PTR OffsetToSimpleString;
    };
} COUNTED_REASON_CONTEXT_RELATIVE, *PCOUNTED_REASON_CONTEXT_RELATIVE;
#endif

typedef struct _PROCESSOR_POWER_INFORMATION
{
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

typedef struct _SYSTEM_POWER_INFORMATION
{
    ULONG MaxIdlenessAllowed;
    ULONG Idleness;
    ULONG TimeRemaining;
    UCHAR CoolingMode;
} SYSTEM_POWER_INFORMATION, *PSYSTEM_POWER_INFORMATION;

NTSYSCALLAPI
NTSTATUS
NTAPI
NtPowerInformation(
    _In_ POWER_INFORMATION_LEVEL InformationLevel,
    _In_reads_bytes_opt_(InputBufferLength) PVOID InputBuffer,
    _In_ ULONG InputBufferLength,
    _Out_writes_bytes_opt_(OutputBufferLength) PVOID OutputBuffer,
    _In_ ULONG OutputBufferLength
    );

NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetThreadExecutionState(
    _In_ EXECUTION_STATE NewFlags, // ES_* flags
    _Out_ EXECUTION_STATE *PreviousFlags
    );

NTSYSCALLAPI
NTSTATUS
NTAPI
NtRequestWakeupLatency(
    _In_ LATENCY_TIME latency
    );

NTSYSCALLAPI
NTSTATUS
NTAPI
NtInitiatePowerAction(
    _In_ POWER_ACTION SystemAction,
    _In_ SYSTEM_POWER_STATE LightestSystemState,
    _In_ ULONG Flags, // POWER_ACTION_* flags
    _In_ BOOLEAN Asynchronous
    );

NTSYSCALLAPI
NTSTATUS
NTAPI
NtSetSystemPowerState(
    _In_ POWER_ACTION SystemAction,
    _In_ SYSTEM_POWER_STATE LightestSystemState,
    _In_ ULONG Flags // POWER_ACTION_* flags
    );

NTSYSCALLAPI
NTSTATUS
NTAPI
NtGetDevicePowerState(
    _In_ HANDLE Device,
    _Out_ PDEVICE_POWER_STATE State
    );

NTSYSCALLAPI
BOOLEAN
NTAPI
NtIsSystemResumeAutomatic(
    VOID
    );

#endif
