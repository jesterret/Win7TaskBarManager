#include <windows.h>
#include <ShlObj.h>
#include "Signature.h"

#pragma pack(push, 4)
struct THUMBBUTTON32
{
	THUMBBUTTONMASK dwMask;
	UINT iId;
	UINT iBitmap;
	HICON hIcon;
	WCHAR szTip[260];
	THUMBBUTTONFLAGS dwFlags;
};
#pragma pack(pop)
class VTable
{
	void** VFuncPtr;
};

class CTray // : CImpWndProc, IStartButtonSite, public ITrayDeskBand, IMessageFilter, public IAppResolverNotify if I get around to defining those interfaces
{
public:
	VTable* CImpWndProcVtbl;
	HWND hTrayWnd;
	VTable* IStartButtonSiteVtbl;
	VTable* ITrayDeskBandVtbl;
	VTable* IMessageFilterVtbl;
	VTable* IAppResolverNotifyVtbl;
	DWORD Unknown[8]; // Some BOOL flags about settings changes I believe
	HWND hActiveWindow;
	IBandSite* TaskBandSite; // Actually CTrayBandSite : IBandSite, IClassFactory
	DWORD AutoHideState; // 0 - disabled, 1 - shown, 3 - hidden
	// More taskbar information like size in pixels etc. 
	// overall size of class is 0xD54 on win7x64 I believe
};

template<typename T> class TrayDataArray
{
public:
	DWORD Count;
	T* Item;
}; 

enum _TBPSTATE
{
	TBP_NORMAL = 0,
	TBP_PAUSED = 1,
	TBP_ERROR = 2,
	TBP_INDETERMINATE = 0xFFFF // Not sure and cba to check properly atm
};
class ITaskItem : IUnknown
{
public:
	virtual long Initialize(HWND) = NULL;
	virtual unsigned long GetFirstOpenedTime() = NULL;
	virtual unsigned long GetLastActivatedTime() = NULL;
	virtual long UpdateLastActivatedTime() = NULL;
	virtual unsigned long GetLastInteractiveStartTime() = NULL;
	virtual long SetLastInteractiveStartTime(unsigned long) = NULL;
	virtual int HasBeenInteracted() = NULL;
	virtual int GetIconPref() = NULL;
	virtual long SetIconPref(int) = NULL;
	virtual int GetIconId() = NULL;
	virtual long SetIconId(int) = NULL;
	virtual long GetHotTrackColor(unsigned long *) = NULL;
	virtual long SetHotTrackColor(unsigned long) = NULL;
	virtual int IsMarkedFullscreen() = NULL;
	virtual long MarkFullscreen(int) = NULL;
	virtual long SetWindow(HWND) = NULL;
	virtual wchar_t const *GetExeName() = NULL;
	virtual HWND GetThumbnailWindow() = NULL;
	virtual HWND GetLivePreviewWindow() = NULL;
	virtual unsigned long GetFlags() = NULL;
	virtual long UpdateFlags(unsigned long, unsigned long) = NULL;
	virtual int SetProgressValue(unsigned long) = NULL;
	virtual unsigned long GetProgressValue() = NULL;
	virtual int SetProgressState(enum _TBPSTATE) = NULL;
	virtual int IsItemNew() = NULL;
	virtual int ResetProgress() = NULL;
	virtual int GetNumTabs() = NULL;
	virtual long AddTab(ITaskItem *) = NULL;
	virtual long RemoveTab(ITaskItem *) = NULL;
	virtual long SetActiveTabItem(ITaskItem *) = NULL;
	virtual long SetTabContainerItem(ITaskItem *) = NULL;
	virtual long SetTabProperties(enum STPFLAG) = NULL;
	virtual ITaskItem *GetActiveTabItem() = NULL;
	virtual ITaskItem *GetTabContainerItem() = NULL;
	virtual int IsActiveTab() = NULL;
	virtual long ThumbBarAddButtons(THUMBBUTTON32 *, unsigned long) = NULL;
	virtual long ThumbBarUpdateButton(THUMBBUTTON32 *) = NULL;
	virtual long ThumbBarSetImageList(HIMAGELIST) = NULL;
	virtual THUMBBUTTON32 *GetThumbButtons(int *) = NULL;
	virtual long GetThumbButtonById(unsigned int, THUMBBUTTON32 *) = NULL;
	virtual HIMAGELIST ThumbBarGetImageList() = NULL;
	virtual int HasThumbBar(unsigned int *) = NULL;
	virtual unsigned int GetThumbButtonThemePartById(unsigned int) = NULL;
	virtual long SetOverlayIcon(HICON) = NULL;
	virtual long SetOverlayIconDescription(unsigned short const *) = NULL;
	virtual HICON GetOverlayIcon(void) = NULL;
	virtual wchar_t const *GetOverlayIconDescription() = NULL;
	virtual unsigned long GetOverlayIconTime(void) = NULL;
	virtual long SetThumbnailToolTip(unsigned short const *) = NULL;
	virtual wchar_t const *GetThumbnailToolTip() = NULL;
	virtual long SetThumbnailClip(RECT&) = NULL;
	virtual long GetThumbnailClip(RECT *) = NULL;

private: // Would not trust those to be portable
	HWND hThumbWnd;
	DWORD Flags;
	DWORD LastActiveTime;
	DWORD FirstOpenedTime;
	DWORD LastInteractStartTime;
	DWORD IconPref;
	DWORD IconID;
	DWORD HotTrackColor;
	DWORD ProgressValue;
	DWORD ProgressState;
	DWORD Unknown1;
	void* TabContainer;
	void* ActiveTabItem;
	QWORD TabCount;
	THUMBBUTTON32* Buttons;
	DWORD ButtonCount;
	DWORD Unknown2;
	HIMAGELIST ButtonImageList;
	DWORD Unknown3[8];
	HICON OverlayIcon;
	DWORD OverlayIconTime;
	DWORD Unknown4;
	WCHAR* OverlayIconDescription;
	WCHAR* ThumbnailTooltip;
	WCHAR* ThumbnailClip;
	DWORD Unknown5[4];
};
class ITaskGroup : IUnknown
{
public:
	virtual long Initialize(ITEMIDLIST_ABSOLUTE const *, unsigned short const *, unsigned short const *) = NULL;
	virtual long AddTaskItem(ITaskItem *, ITaskItem *) = NULL;
	virtual long RemoveTaskItem(ITaskItem *) = NULL;
	virtual long EnumTaskItems(class IEnumTaskItems **pEnumerator) = NULL;
	virtual long DoesWindowMatch(HWND *, ITEMIDLIST_ABSOLUTE const *, unsigned short const *, enum WINDOWMATCHCONFIDENCE *, ITaskItem **) = NULL;
	virtual long GetItemFromWindow(HWND *, ITaskItem **) = NULL;
	virtual long GetNumItems(int *) = NULL;
	virtual long GetNumTabs(int *, int *) = NULL;
	virtual long GetTitleText(ITaskItem *, wchar_t *, int) = NULL;
	virtual long GetToolTipText(ITaskItem *, wchar_t *, int) = NULL;
	virtual long GetLauncherName(unsigned short **) = NULL;
	virtual long GetIconPref(ITaskItem *, int *) = NULL;
	virtual long SetIconPref(ITaskItem *, int) = NULL;
	virtual long GetIconId(ITaskItem *, int *) = NULL;
	virtual long SetIconId(ITaskItem *, int) = NULL;
	virtual long GetHotTrackColor(ITaskItem *, unsigned long *) = NULL;
	virtual long SetHotTrackColor(ITaskItem *, unsigned long) = NULL;
	virtual const wchar_t *GetExeName(void) = NULL;
	virtual long SetExeName(unsigned short const *) = NULL;
	virtual unsigned short const *  GetIconResource(void) = NULL;
	virtual unsigned short const *  GetAppID(void) = NULL;
	virtual unsigned short const *  GetAppIDForSQM(void) = NULL;
	virtual long SetAppID(unsigned short const *) = NULL;
	virtual int ShouldLog(void) = NULL;
	virtual unsigned long GetFlags(void) = NULL;
	virtual long UpdateFlags(unsigned long, unsigned long) = NULL;
	virtual long GetLastActiveItem(ITaskItem **) = NULL;
	virtual long GroupMenuCommand(int) = NULL;
	virtual long SetTip(wchar_t const *) = NULL;
	virtual struct ITEMIDLIST_ABSOLUTE const *GetShortcutIDList(void) = NULL;
	virtual long SetShortcutIDList(ITEMIDLIST_ABSOLUTE const *) = NULL;
	virtual struct ITEMIDLIST_ABSOLUTE *GetApplicationIDList(void) = NULL;
	virtual long RemoveTabs(ITaskItem *, int, void(__cdecl*)(unsigned __int64, struct HWND__ *), unsigned __int64) = NULL;
	virtual struct HICON*  GetOverlayIcon(void) = NULL;
	virtual unsigned short const *  GetOverlayIconDescription(void) = NULL;
	virtual int IsAppPinnable(void) = NULL;
	virtual int IsAppLaunchable(void) = NULL;
	virtual int HasDestinations(void) = NULL;
	virtual int HasDestinationsMenu(void) = NULL;
	virtual long SetPinnableAndLaunchable(int) = NULL;
};
class IEnumTaskItems : IUnknown
{
public:
	virtual HRESULT Next(ITaskItem**) = NULL;
	virtual HRESULT Reset(void) = NULL;
};

enum PINNABLEFLAG 
{
	PINNABLE_EXEONLY = 1,
	PINNABLE_REJECTSLOWMEDIA = 2,
	PINNABLE_DETERMINE_APPLICATION = 4,
	PINNABLE_APPLICATION_ONLY = 8
};
class IPinnedList : IUnknown
{
	HRESULT EnumObjects(IEnumFullIDList **);
	HRESULT Modify(PCIDLIST_ABSOLUTE, PCIDLIST_ABSOLUTE);
	HRESULT GetChangeCount(ULONG *);
	HRESULT IsPinnable(IDataObject *, ULONG, PIDLIST_ABSOLUTE *);
	HRESULT Resolve(HWND, ULONG, PCIDLIST_ABSOLUTE, PIDLIST_ABSOLUTE *);
	HRESULT IsPinned(PCIDLIST_ABSOLUTE);
	HRESULT GetPinnableInfo(IDataObject *, PINNABLEFLAG, IShellItem2 **, IShellItem **, PWSTR *, INT *);
	HRESULT IsPinnable(IDataObject *, PINNABLEFLAG);
	HRESULT GetPinnedItem(PCIDLIST_ABSOLUTE, PIDLIST_ABSOLUTE *);
	HRESULT GetAppIDForPinnedItem(PCIDLIST_ABSOLUTE, PWSTR *);
	HRESULT ItemChangeNotify(PCIDLIST_ABSOLUTE, PCIDLIST_ABSOLUTE);
	HRESULT UpdateForRemovedItemsAsNecessary(VOID);
};

class ITaskListUI : IUnknown
{
	virtual long TaskCreated(ITaskGroup *, ITaskItem *) = NULL;
	virtual long GetMinRect(ITaskGroup *, ITaskItem *, struct tagRECT *) = NULL;
	virtual long ActivateTask(ITaskGroup *, ITaskItem *) = NULL;
	virtual long RedrawTask(ITaskGroup *, ITaskItem *, unsigned long) = NULL;
	virtual long TaskDestroyed(ITaskGroup *, ITaskItem *) = NULL;
	virtual long TaskActiveTabUpdated(ITaskGroup *, ITaskItem *, ITaskItem *, ITaskItem *) = NULL;
	virtual long RedrawExtendedTask(ITaskGroup *, ITaskItem *) = NULL;
	virtual long TaskReordered(ITaskGroup *, ITaskItem *) = NULL;
	virtual int GetButtonHeight(int *) = NULL;
	virtual int GetMinButtonWidth() = NULL;
	virtual void GetButtonMargin(int *, int *) = NULL;
	virtual HWND GetHWND() = NULL;
	virtual long AutoSize() = NULL;
	virtual long GetTrackedTask(ITaskGroup * *, ITaskItem * *) = NULL;
	virtual long GetActiveTask(ITaskGroup * *, ITaskItem * *) = NULL;
	virtual long SettingsChanged() = NULL;
	virtual long ReorderTaskGroup(ITaskGroup *, ITaskGroup *) = NULL;
	virtual long SetRelativeTaskOrder(ITaskGroup *, int) = NULL;
	virtual int GetRelativeTaskOrder(ITaskGroup *) = NULL;
	virtual void HandleScrollToPostion(int) = NULL;
	virtual void HandleTaskGroupPinned(ITaskGroup *) = NULL;
	virtual HWND GetDestinationMenuWindow() = NULL;
	virtual long SetDrag(int) = NULL;
	virtual void RemoveIconForAllButtons() = NULL;
	virtual long DismissHoverUI(int) = NULL;
	virtual long DismissPopupMenu() = NULL;
	virtual int PermitAutoHide() = NULL;
	virtual long ShowDestinationMenu(ITaskGroup *, ITaskItem *, ITEMIDLIST_ABSOLUTE const *, int) = NULL;
	virtual long DismissDestinationMenu(ITaskGroup *) = NULL;
	virtual long HandleAccelerator(MSG *) = NULL;
	virtual long NavigateFromKey(int, unsigned int) = NULL;
	virtual long HandleWinNumHotKey(short, unsigned short) = NULL;
	virtual long SwitchToTracked() = NULL;
	virtual long UpdateThumbBarButton(ITaskGroup *, ITaskItem *, unsigned int) = NULL;
	virtual long UpdateThumbBarImageList(ITaskGroup *, ITaskItem *) = NULL;
	virtual long TaskProgressUpdate(ITaskGroup *, ITaskItem *, int) = NULL;
	virtual long UpdateOverlayIcon(ITaskGroup *, ITaskItem *, HICON) = NULL;
	virtual void RealityCheck() = NULL;
	virtual int IsExtendedUIVisible() = NULL;
};
class ITaskBand
{
public:
	VTable* IDeskBand2;
	VTable* IObjectWithSite;
	VTable* IInputObject;
	VTable* IPersistStream;
	VTable* IWinEventHandler;
	VTable* IOleCommandTarget;
	VTable* ITaskListManager;
	DWORD RefCount;
	DWORD Val1;
	DWORD Val2;
	void* xxx, *xxy;
	DWORD Unknown[4]; // 1 on x86
	CTray* ParentTray;
	DWORD Padding;
	void* UnkVal;
	DWORD UnknownX; 
	DWORD UnknownY;
	void* UnkVal2;
	IImageList* ImageList;
	IImageList* SparseImageList;
	DWORD Unknown3;
	IPinnedList* TaskbandPin;
	DWORD Unknown4;
	IShellTaskScheduler* Sheduler;
	IShellTaskScheduler* Sheduler2;
	DWORD Unknown5[4];
	void* Unknown6;
	IBandSite* NavBar;
	ITaskListUI* TaskListWnd;
	void* Unknown7[3];
	ITaskGroup* LastSelectedGroup;
	TrayDataArray<ITaskGroup*>* TaskGroups;
	// Few remaining bytes pretty useless
};

class ITaskListManager : IUnknown
{
public:
	virtual long GetTaskFromWindow(HWND, ITaskGroup **, ITaskItem **) = NULL;
	virtual long GetIconId(ITaskGroup *, ITaskItem *, int, int *) = NULL;
	virtual long GetImageList(unsigned int, HIMAGELIST *) = NULL;
	virtual long SwitchTo(ITaskItem *, int) = NULL;
	virtual long Launch(ITaskGroup *, POINT *, BOOL) = NULL;
	virtual long AddGhostPinnedItem(IDataObject *, ITaskGroup **, struct IShellItem **) = NULL;
	virtual long CanPinDestinationToTarget(IDataObject *, ITaskGroup *, struct IShellItem **, struct IShellItem **, unsigned short **, int) = NULL;
	virtual long RemoveGhostPinnedItem(ITaskGroup *) = NULL;
	virtual long RemoveInvalidTabs(ITaskGroup *) = NULL;
	virtual long InsertPinnedItem(class ITaskListUI *, IDataObject *, ITaskGroup *, int) = NULL;
	virtual void CloseWnd(HWND) = NULL;
	virtual void CloseItem(ITaskItem *) = NULL;
	virtual void RemoveIcon(ITaskItem *) = NULL;
	virtual void RemoveGroupIcon(ITaskGroup *) = NULL;
	virtual void StuckTrayMonitorChanged(HMONITOR) = NULL;
	virtual long ComputeTaskbarForNavigate(class ITaskListUI *, int, class ITaskListUI **) = NULL;
	virtual void HandleJumpViewClose(struct JUMPVIEWCLOSEDATA const *) = NULL;
	virtual long GetOrCreateBadgeVisualForApp(unsigned short const *, struct IBadgeVisualProperties *, unsigned int, HBITMAP *) = NULL;
	virtual long GetAccessibleStringForBadge(struct IBadgeData *, unsigned short **) = NULL;
	virtual void GetIconSize(DWORD, SIZE *) = NULL;
	virtual long GetGroupByTime(unsigned long, enum eTIMEFLAGS, ITaskGroup **, unsigned long *) = NULL;
	virtual void TaskOrderChanged() = NULL;
	virtual void TaskOrderChanged(ITaskGroup *, int) = NULL;
	virtual unsigned long GetMultimonStuckPlace() = NULL;
	virtual long GetPearlRectFromMonitor(HMONITOR, RECT *, unsigned long *) = NULL;
	virtual void HandleImmersiveInvokeOnMonitor(HMONITOR) = NULL;
	virtual long RequestContextMenu(ITaskGroup *, ITaskItem *, int, IContextMenu **, HMENU *) = NULL;
	virtual long HandleContextMenuCommand(struct ITaskItemFilter *, ITaskGroup *, HWND, IContextMenu *, int) = NULL;
};

DWORD WINAPI OnAttach(LPVOID hModule)
{
	Pattern Win7Pat[2] =
	{
		Pattern(L"B9 [? ? ? ? 33 F6", PatternType::Pointer), // x86 Win 7, working on it
		Pattern(L"48 8D 0D [? ? ? ? E8 ? ? ? ? 48", PatternType::RelativePointer) // x64 Win 7
	};
	auto pTray = (CTray*)PatternScanner::FindPattern(Win7Pat[1]);
	if (pTray != nullptr && pTray->TaskBandSite != nullptr)
	{
		IBandSite* site = pTray->TaskBandSite;
		DWORD unused = 0;
		for (DWORD count = site->EnumBands(-1, &unused), i = 0; i < count; i++)
		{
			DWORD id = 0;
			auto hr0 = site->EnumBands(i, &id);

			//GUID IID_ITaskListManager;
			//CLSIDFromString(L"{1a64fc25-5b96-4c6a-a37d-d32fdce4e57d}", &IID_ITaskListManager);
			// ITaskListManager* band = nullptr;
			// Not using the Vtable since it's more limited (probably to limit abuse of unsafe objects ;))

			IDeskBand2* band = nullptr;
			auto hr = site->GetBandObject(id, IID_IDeskBand2, (void**)&band); // It's not actually the base of the object, it also implements CImpWndProc class, but whatever
			if (band)
			{
				auto group = ((ITaskBand*)band)->TaskGroups;
				if (group)
				{
					for (DWORD i = 0; i < group->Count; i++)
					{
						auto item = group->Item[i];
						if (item)
						{
							IEnumTaskItems* iter = nullptr;
							ITaskItem* it = nullptr;
							item->EnumTaskItems(&iter);
							while (iter->Next(&it) == S_OK)
							{
								if (it != nullptr)
								{
									// Manipulate task item
								}
							}
						}
					}
				}
			}
		}
	}
	FreeLibraryAndExitThread(HMODULE(hModule), 0);
	return 0;
}

BOOL CALLBACK DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	if (_Reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, OnAttach, _DllHandle, 0, 0);
	}
	return TRUE;
}