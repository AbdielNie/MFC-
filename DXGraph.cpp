#include "StdAfx.h"
#include "DXGraph.h"


////////////////////////////////////////////////////////////////////////////////
CDXGraph::CDXGraph()
{
	mGraph        = NULL;
	mMediaControl = NULL;
	mEvent        = NULL;
	mBasicAudio   = NULL;
	mSeeking      = NULL;

	mObjectTableEntry = 0;
}

CDXGraph::~CDXGraph()
{
	Release();
}

bool CDXGraph::Create(void)
{
	if (!mGraph)
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder, (void **)&mGraph)))
		{
			AddToObjectTable();

			return QueryInterfaces();
		}
		mGraph = 0;
	}

	return false;
}

bool CDXGraph::QueryInterfaces(void)
{
	if (mGraph)
	{
		HRESULT hr = NOERROR;
		hr |= mGraph->QueryInterface(IID_IMediaControl, (void **)&mMediaControl);
		hr |= mGraph->QueryInterface(IID_IMediaEventEx, (void **)&mEvent);
		hr |= mGraph->QueryInterface(IID_IBasicAudio, (void **)&mBasicAudio);
		hr |= mGraph->QueryInterface(IID_IMediaSeeking, (void **)&mSeeking);
		if (mSeeking)
		{
			mSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
		}
		return SUCCEEDED(hr);
	}
	return false;
}

void CDXGraph::Release(void)
{
	if (mSeeking)
	{
		mSeeking->Release();
		mSeeking = NULL;
	}
	if (mMediaControl)
	{
		mMediaControl->Release();
		mMediaControl = NULL;
	}
	if (mEvent)
	{
		mEvent->Release();
		mEvent = NULL;
	}
	if (mBasicAudio)
	{
		mBasicAudio->Release();
		mBasicAudio = NULL;
	}
	RemoveFromObjectTable();
	if (mGraph) 
	{
		mGraph->Release(); 
		mGraph = NULL;
	}
}

bool CDXGraph::Attach(IGraphBuilder * inGraphBuilder)
{
	Release();

	if (inGraphBuilder)
	{
		inGraphBuilder->AddRef();
		mGraph = inGraphBuilder;

		AddToObjectTable();
		return QueryInterfaces();
	}
	return true;
}

IGraphBuilder * CDXGraph::GetGraph(void)
{
	return mGraph;
}

IMediaEventEx * CDXGraph::GetEventHandle(void)
{
	return mEvent;
}

// Connect filter from the upstream output pin to the downstream input pin
bool CDXGraph::ConnectFilters(IPin * inOutputPin, IPin * inInputPin, 
	const AM_MEDIA_TYPE * inMediaType)
{
	if (mGraph && inOutputPin && inInputPin)
	{
		HRESULT hr = mGraph->ConnectDirect(inOutputPin, inInputPin, inMediaType);
		return SUCCEEDED(hr) ? true : false;
	}
	return false;
}

void CDXGraph::DisconnectFilters(IPin * inOutputPin)
{
	if (mGraph && inOutputPin)
	{
		HRESULT hr = mGraph->Disconnect(inOutputPin);
	}
}

bool CDXGraph::SetNotifyWindow(HWND inWindow)
{
	if (mEvent)
	{
		mEvent->SetNotifyWindow((OAHWND)inWindow, WM_GRAPHNOTIFY, 0);
		return true;
	}
	return false;
}

void CDXGraph::HandleEvent(WPARAM inWParam, LPARAM inLParam)
{
	if (mEvent)
	{
		LONG eventCode = 0, eventParam1 = 0, eventParam2 = 0;
		while (SUCCEEDED(mEvent->GetEvent(&eventCode, &eventParam1, &eventParam2, 0)))
		{
			mEvent->FreeEventParams(eventCode, eventParam1, eventParam2);
			switch (eventCode)
			{
			case EC_COMPLETE:
				break;

			case EC_USERABORT:
			case EC_ERRORABORT:
				break;

			default:
				break;
			}
		}
	}
}

bool CDXGraph::Run(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsRunning())
		{
			if (SUCCEEDED(mMediaControl->Run()))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool CDXGraph::Stop(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsStopped())
		{	
			if (SUCCEEDED(mMediaControl->Stop()))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool CDXGraph::Pause(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsPaused())
		{	
			if (SUCCEEDED(mMediaControl->Pause()))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool CDXGraph::IsRunning(void)
{
	if (mGraph && mMediaControl)
	{
		OAFilterState state = State_Stopped;
		if (SUCCEEDED(mMediaControl->GetState(10, &state)))
		{
			return state == State_Running;
		}
	}
	return false;
}

bool CDXGraph::IsStopped(void)
{
	if (mGraph && mMediaControl)
	{
		OAFilterState state = State_Stopped;
		if (SUCCEEDED(mMediaControl->GetState(10, &state)))
		{
			return state == State_Stopped;
		}
	}
	return false;
}

bool CDXGraph::IsPaused(void)
{
	if (mGraph && mMediaControl)
	{
		OAFilterState state = State_Stopped;
		if (SUCCEEDED(mMediaControl->GetState(10, &state)))
		{
			return state == State_Paused;
		}
	}
	return false;
}

// IMediaSeeking features
bool CDXGraph::GetCurrentPosition(double * outPosition)
{
	if (mSeeking)
	{
		__int64 position = 0;
		if (SUCCEEDED(mSeeking->GetCurrentPosition(&position)))
		{
			*outPosition = ((double)position) / 10000000.;
			return true;
		}
	}
	return false;
}

bool CDXGraph::GetStopPosition(double * outPosition)
{
	if (mSeeking)
	{
		__int64 position = 0;
		if (SUCCEEDED(mSeeking->GetStopPosition(&position)))
		{
			*outPosition = ((double)position) / 10000000.;
			return true;
		}
	}
	return false;
}

bool CDXGraph::SetCurrentPosition(double inPosition)
{
	if (mSeeking)
	{
		__int64 one = 10000000;
		__int64 position = (__int64)(one * inPosition);
		HRESULT hr = mSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, 
			0, AM_SEEKING_NoPositioning);
		return SUCCEEDED(hr);
	}
	return false;
}

bool CDXGraph::SetStartStopPosition(double inStart, double inStop)
{
	if (mSeeking)
	{
		__int64 one = 10000000;
		__int64 startPos = (__int64)(one * inStart);
		__int64 stopPos  = (__int64)(one * inStop);
		HRESULT hr = mSeeking->SetPositions(&startPos, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, 
			&stopPos, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame);
		return SUCCEEDED(hr);
	}
	return false;
}

bool CDXGraph::GetDuration(double * outDuration)
{
	if (mSeeking)
	{
		__int64 length = 0;
		if (SUCCEEDED(mSeeking->GetDuration(&length)))
		{
			*outDuration = ((double)length) / 10000000.;
			return true;
		}
	}
	return false;
}

bool CDXGraph::SetPlaybackRate(double inRate)
{
	if (mSeeking)
	{
		if (SUCCEEDED(mSeeking->SetRate(inRate)))
		{
			return true;
		}
	}
	return false;
}

// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
bool CDXGraph::SetAudioVolume(long inVolume)
{
	if (mBasicAudio)
	{
		HRESULT hr = mBasicAudio->put_Volume(inVolume);
		return SUCCEEDED(hr);
	}
	return false;
}

long CDXGraph::GetAudioVolume(void)
{
	long volume = 0;
	if (mBasicAudio)
	{
		mBasicAudio->get_Volume(&volume);
	}
	return volume;
}

// Attention: range from -10000(left) to 10000(right), and 0 is both.
bool CDXGraph::SetAudioBalance(long inBalance)
{
	if (mBasicAudio)
	{
		HRESULT hr = mBasicAudio->put_Balance(inBalance);
		return SUCCEEDED(hr);
	}
	return false;
}

long CDXGraph::GetAudioBalance(void)
{
	long balance = 0;
	if (mBasicAudio)
	{
		mBasicAudio->get_Balance(&balance);
	}
	return balance;
}

bool CDXGraph::RenderFile(const WCHAR * inFile)
{
	if (mGraph)
	{
		if (SUCCEEDED(mGraph->RenderFile(inFile, NULL)))
		{
			return true;
		}
	}
	return false;
}

//////////////////////// For GraphEdit Dubug purpose /////////////////////////////
void CDXGraph::AddToObjectTable(void)
{
	IMoniker * pMoniker = 0;
	IRunningObjectTable * objectTable = 0;
	if (SUCCEEDED(GetRunningObjectTable(0, &objectTable))) 
	{
		WCHAR wsz[256];
		wsprintfW(wsz, L"FilterGraph %08p pid %08x", (DWORD_PTR)mGraph, GetCurrentProcessId());
		HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
		if (SUCCEEDED(hr)) 
		{
			hr = objectTable->Register(0, mGraph, pMoniker, &mObjectTableEntry);
			pMoniker->Release();
		}
		objectTable->Release();
	}
}

void CDXGraph::RemoveFromObjectTable(void)
{
	IRunningObjectTable * objectTable = 0;
	if (SUCCEEDED(GetRunningObjectTable(0, &objectTable))) 
	{
		objectTable->Revoke(mObjectTableEntry);
		objectTable->Release();
		mObjectTableEntry = 0;
	}
}
