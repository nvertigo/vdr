/*
 * menu.h: The actual menu implementations
 *
 * See the main source file 'vdr.c' for copyright information and
 * how to reach the author.
 *
 * $Id: menu.h 4.8 2018/04/14 10:24:41 kls Exp $
 */

#ifndef __MENU_H
#define __MENU_H

#include "ci.h"
#include "device.h"
#include "epg.h"
#include "osdbase.h"
#include "dvbplayer.h"
#include "menuitems.h"
#include "recorder.h"
#include "skins.h"

class cMenuText : public cOsdMenu {
private:
  char *text;
  eDvbFont font;
public:
  cMenuText(const char *Title, const char *Text, eDvbFont Font = fontOsd);
  virtual ~cMenuText();
  void SetText(const char *Text);
  virtual void Display(void);
  virtual eOSState ProcessKey(eKeys Key);
  };

class cMenuFolder : public cOsdMenu {
private:
  cNestedItemList *nestedItemList;
  cList<cNestedItem> *list;
  cString dir;
  cOsdItem *firstFolder;
  bool editing;
  int helpKeys;
  void SetHelpKeys(void);
  void Set(const char *CurrentFolder = NULL);
  void DescendPath(const char *Path);
  eOSState SetFolder(void);
  eOSState Select(bool Open);
  eOSState New(void);
  eOSState Delete(void);
  eOSState Edit(void);
  cMenuFolder(const char *Title, cList<cNestedItem> *List, cNestedItemList *NestedItemList, const char *Dir, const char *Path = NULL);
public:
  cMenuFolder(const char *Title, cNestedItemList *NestedItemList, const char *Path = NULL);
  cString GetFolder(void);
  virtual eOSState ProcessKey(eKeys Key);
  };

class cMenuCommands : public cOsdMenu {
private:
  cList<cNestedItem> *commands;
  cString parameters;
  cString title;
  cString command;
  bool confirm;
  char *result;
  bool Parse(const char *s);
  eOSState Execute(void);
public:
  cMenuCommands(const char *Title, cList<cNestedItem> *Commands, const char *Parameters = NULL);
  virtual ~cMenuCommands();
  virtual eOSState ProcessKey(eKeys Key);
  };

class cMenuEditTimer : public cOsdMenu {
private:
  static const cTimer *addedTimer;
  cTimer *timer;
  cTimer data;
  int channel;
  bool addIfConfirmed;
  cStringList svdrpServerNames;
  char remote[HOST_NAME_MAX];
  cMenuEditStrItem *file;
  cMenuEditDateItem *day;
  cMenuEditDateItem *firstday;
  eOSState SetFolder(void);
  void SetFirstDayItem(void);
  void SetHelpKeys(void);
public:
  cMenuEditTimer(cTimer *Timer, bool New = false);
  virtual ~cMenuEditTimer();
  virtual eOSState ProcessKey(eKeys Key);
  static const cTimer *AddedTimer(void);
  };

class cMenuEvent : public cOsdMenu {
private:
  const cEvent *event;
public:
  cMenuEvent(const cTimers *Timers, const cChannels *Channels, const cEvent *Event, bool CanSwitch = false, bool Buttons = false);
  virtual void Display(void);
  virtual eOSState ProcessKey(eKeys Key);
  };

class cMenuMain : public cOsdMenu {
private:
  bool replaying;
  cOsdItem *stopReplayItem;
  cOsdItem *cancelEditingItem;
  cOsdItem *stopRecordingItem;
  int recordControlsState;
  static cOsdObject *pluginOsdObject;
  void Set(void);
  bool Update(bool Force = false);
public:
  cMenuMain(eOSState State = osUnknown, bool OpenSubMenus = false);
  virtual eOSState ProcessKey(eKeys Key);
  static cOsdObject *PluginOsdObject(void);
  };

class cDisplayChannel : public cOsdObject {
private:
  int group;
  bool withInfo;
  bool timeout;
  const cPositioner *positioner;
  const cEvent *lastPresent;
  const cEvent *lastFollowing;
  static cDisplayChannel *currentDisplayChannel;
  void Refresh(void);
  const cChannel *NextAvailableChannel(const cChannel *Channel, int Direction);
protected:
  cSkinDisplayChannel *displayChannel;
  cTimeMs lastTime;
  int number;
  const cChannel *channel;
  int osdState;
  void DisplayChannel(void);
  void DisplayInfo(void);
public:
  cDisplayChannel(int Number, bool Switched);
  cDisplayChannel(eKeys FirstKey, bool processKey = true);
  virtual ~cDisplayChannel();
  virtual eOSState ProcessKey(eKeys Key);
  static bool IsOpen(void) { return currentDisplayChannel != NULL; }
  };

enum eExtendedState {
  esInit = 0,
  esDefault,
  esChannelInfo,
  esChannelList,
  esChannelListInfo,
  esGroupsList,
  esGroupsChannelList,
  esGroupsChannelListInfo,
  esClose
  };

class cChannelListItem : public cListObject {
private:
  const cChannel *channel;
public:
  cChannelListItem(const cChannel *Channel) { channel = Channel; };
  virtual ~cChannelListItem(void) { };
  const cChannel *Channel(void) { return channel; }
  };

class cGroupListItem : public cListObject {
private:
  const cChannel *channel;
  int numChannels;
public:
  cGroupListItem(const cChannel *Channel) { channel = Channel; numChannels = 0; };
  virtual ~cGroupListItem(void) { };
  const char *GroupName(void);
  void SetNumChannels(int NumChannels) { numChannels = NumChannels; };
  int NumChannels(void) { return numChannels; };
  const cChannel *Channel(void) { return channel; }
  };

class cDisplayChannelExtended : public cDisplayChannel {
private:
  eExtendedState state;
  int keyRightOpensChannellist;
  int numItemsChannel, startChannel, currentChannel;
  int numItemsGroup, startGroup, currentGroup;
  cList<cChannelListItem> channellist;
  cList<cGroupListItem> grouplist;
  void StateNumberKey(int key, cSkinDisplayChannelExtended *dcExt);
  bool StateInit(int key, cSkinDisplayChannelExtended *dcExt);
  bool StateDefault(int key, cSkinDisplayChannelExtended *dcExt);
  bool StateChannelInfo(int key, cSkinDisplayChannelExtended *dcExt);
  bool StateChannelList(int key, cSkinDisplayChannelExtended *dcExt);
  bool StateGroupList(int key, cSkinDisplayChannelExtended *dcExt);
  bool StateGroupChannelList(int key, cSkinDisplayChannelExtended *dcExt);
  void ShowChannellistInfo(cSkinDisplayChannelExtended *dcExt, eDisplaychannelView newViewType);
  void InitChannelList(cSkinDisplayChannelExtended *dcExt);
  void SetChannelList(void);
  int GetIndexChannel(const cChannel *c);
  void InitGroupList(cSkinDisplayChannelExtended *dcExt);
  void SetGroupList(void);
  int GetIndexGroup(const cChannel *c);
  void InitGroupChannelList(cSkinDisplayChannelExtended *dcExt);
  void SetGroupChannelList(cSkinDisplayChannelExtended *dcExt);
  void CursorUp(cSkinDisplayChannelExtended *dcExt);
  void CursorDown(cSkinDisplayChannelExtended *dcExt);
  void DisplayChannelList(cSkinDisplayChannelExtended *dcExt);
  void DisplayGroupList(cSkinDisplayChannelExtended *dcExt);
  bool SwitchChannel(void);
  const cChannel *LastChannelSep(void);
public:
  cDisplayChannelExtended(int Number, bool Switched);
  cDisplayChannelExtended(eKeys FirstKey);
  virtual ~cDisplayChannelExtended();
  virtual eOSState ProcessKey(eKeys Key);
  };

class cDisplayVolume : public cOsdObject {
private:
  cSkinDisplayVolume *displayVolume;
  cTimeMs timeout;
  static cDisplayVolume *currentDisplayVolume;
  virtual void Show(void);
  cDisplayVolume(void);
public:
  virtual ~cDisplayVolume();
  static cDisplayVolume *Create(void);
  static void Process(eKeys Key);
  eOSState ProcessKey(eKeys Key);
  };

class cDisplayTracks : public cOsdObject {
private:
  cSkinDisplayTracks *displayTracks;
  cTimeMs timeout;
  eTrackType types[ttMaxTrackTypes];
  char *descriptions[ttMaxTrackTypes + 1]; // list is NULL terminated
  int numTracks, track, audioChannel;
  static cDisplayTracks *currentDisplayTracks;
  virtual void Show(void);
  cDisplayTracks(void);
public:
  virtual ~cDisplayTracks();
  static bool IsOpen(void) { return currentDisplayTracks != NULL; }
  static cDisplayTracks *Create(void);
  static void Process(eKeys Key);
  eOSState ProcessKey(eKeys Key);
  };

class cDisplaySubtitleTracks : public cOsdObject {
private:
  cSkinDisplayTracks *displayTracks;
  cTimeMs timeout;
  eTrackType types[ttMaxTrackTypes];
  char *descriptions[ttMaxTrackTypes + 1]; // list is NULL terminated
  int numTracks, track;
  static cDisplaySubtitleTracks *currentDisplayTracks;
  virtual void Show(void);
  cDisplaySubtitleTracks(void);
public:
  virtual ~cDisplaySubtitleTracks();
  static bool IsOpen(void) { return currentDisplayTracks != NULL; }
  static cDisplaySubtitleTracks *Create(void);
  static void Process(eKeys Key);
  eOSState ProcessKey(eKeys Key);
  };

cOsdObject *CamControl(void);
bool CamMenuActive(void);

class cRecordingFilter {
public:
  virtual ~cRecordingFilter(void) {};
  virtual bool Filter(const cRecording *Recording) const = 0;
      ///< Returns true if the given Recording shall be displayed in the Recordings menu.
  };

class cMenuRecordingItem;

class cMenuRecordings : public cOsdMenu {
private:
  char *base;
  int level;
  cStateKey recordingsStateKey;
  int helpKeys;
  const cRecordingFilter *filter;
  static cString path;
  static cString fileName;
  void SetHelpKeys(void);
  void Set(bool Refresh = false);
  bool Open(bool OpenSubMenus = false);
  eOSState Play(void);
  eOSState Rewind(void);
  eOSState Delete(void);
  eOSState Info(void);
  eOSState Sort(void);
  eOSState Commands(eKeys Key = kNone);
protected:
  cString DirectoryName(void);
public:
  cMenuRecordings(const char *Base = NULL, int Level = 0, bool OpenSubMenus = false, const cRecordingFilter *Filter = NULL);
  ~cMenuRecordings();
  virtual eOSState ProcessKey(eKeys Key);
  static void SetPath(const char *Path);
  static void SetRecording(const char *FileName);
  };

class cRecordControl {
private:
  cDevice *device;
  cTimer *timer;
  cRecorder *recorder;
  const cEvent *event;
  cString instantId;
  char *fileName;
  bool GetEvent(void);
public:
  cRecordControl(cDevice *Device, cTimers *Timers, cTimer *Timer = NULL, bool Pause = false);
  cRecordControl(cDevice *Device, cTimers *Timers, cTimer *Timer, bool Pause, bool* reused);
  void Construct(cDevice *Device, cTimers *Timers, cTimer *Timer, bool Pause, bool* reused);
  virtual ~cRecordControl();
  bool Process(time_t t);
  cDevice *Device(void) { return device; }
  void Stop(bool ExecuteUserCommand = true);
  const char *InstantId(void) { return instantId; }
  const char *FileName(void) { return fileName; }
  cTimer *Timer(void) { return timer; }
  };

class cRecordControls {
private:
  static cRecordControl *RecordControls[];
  static int state;
public:
  static bool Start(cTimers *Timers, cTimer *Timer, bool Pause = false);
  static bool Start(cTimers *Timers, cTimer *Timer, bool Pause, bool* reused);
  static bool Start(bool Pause = false);
  static void Stop(const char *InstantId);
  static void Stop(cTimer *Timer);
  static bool PauseLiveVideo(void);
  static bool PauseLiveVideo(bool rewind);
  static const char *GetInstantId(const char *LastInstantId);
  static cRecordControl *GetRecordControl(const char *FileName);
  static cRecordControl *GetRecordControl(const cTimer *Timer);
         ///< Returns the cRecordControl for the given Timer.
         ///< If there is no cRecordControl for Timer, NULL is returned.
  static bool Process(cTimers *Timers, time_t t);
  static void ChannelDataModified(const cChannel *Channel);
  static bool Active(void);
  static void Shutdown(void);
  static void ChangeState(void) { state++; }
  static bool StateChanged(int &State);
  };

class cAdaptiveSkipper {
private:
  int *initialValue;
  int currentValue;
  double framesPerSecond;
  eKeys lastKey;
  cTimeMs timeout;
public:
  cAdaptiveSkipper(void);
  void Initialize(int *InitialValue, double FramesPerSecond);
  int GetValue(eKeys Key);
  };

class cReplayControl : public cDvbPlayerControl {
private:
  cSkinDisplayReplay *displayReplay;
  cAdaptiveSkipper adaptiveSkipper;
  cMarks marks;
  bool marksModified;
  bool visible, modeOnly, shown, displayFrames;
  int lastCurrent, lastTotal;
  bool lastPlay, lastForward;
  int lastSpeed;
  time_t timeoutShow;
  cTimeMs updateTimer;
  bool timeSearchActive, timeSearchHide;
  int timeSearchTime, timeSearchPos;
  void TimeSearchDisplay(void);
  void TimeSearchProcess(eKeys Key);
  void TimeSearch(void);
  void ShowTimed(int Seconds = 0);
  static cReplayControl *currentReplayControl;
  static cString fileName;
  void ShowMode(void);
  bool ShowProgress(bool Initial);
  void MarkToggle(void);
  void MarkJump(bool Forward);
  void MarkMove(int Frames, bool MarkRequired);
  void EditCut(void);
  void EditTest(void);
public:
  cReplayControl(bool PauseLive = false);
  cReplayControl(ReplayState replayState);
  void Construct(ReplayState replayState);
  virtual ~cReplayControl();
  void Stop(void);
  virtual cOsdObject *GetInfo(void);
  virtual const cRecording *GetRecording(void);
  virtual eOSState ProcessKey(eKeys Key);
  virtual void Show(void);
  virtual void Hide(void);
  bool Visible(void) { return visible; }
  virtual void ClearEditingMarks(void);
  static void SetRecording(const char *FileName);
  static const char *NowReplaying(void);
  static const char *LastReplayed(void);
  static void ClearLastReplayed(const char *FileName);
  };

#endif //__MENU_H
