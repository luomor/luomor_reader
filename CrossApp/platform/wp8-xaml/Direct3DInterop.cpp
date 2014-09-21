
#include "Direct3DInterop.h"
#include "Direct3DContentProvider.h"
//#include "proj.wp8/EditBoxEvent.h"

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;
using namespace Windows::Graphics::Display;
using namespace DirectX;
using namespace PhoneDirect3DXamlAppComponent;

namespace PhoneDirect3DXamlAppComponent
{

Direct3DInterop::Direct3DInterop() 
    : mCurrentOrientation(DisplayOrientations::Portrait), m_delegate(nullptr)
{
    m_renderer = ref new Cocos2dRenderer();
}


IDrawingSurfaceContentProvider^ Direct3DInterop::CreateContentProvider()
{
    ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
    return reinterpret_cast<IDrawingSurfaceContentProvider^>(provider.Get());
}


// IDrawingSurfaceManipulationHandler
void Direct3DInterop::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
{
    manipulationHost->PointerPressed +=
        ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerPressed);

    manipulationHost->PointerMoved +=
        ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerMoved);

    manipulationHost->PointerReleased +=
        ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerReleased);
}

void Direct3DInterop::UpdateForWindowSizeChange(float width, float height)
{
    m_renderer->UpdateForWindowSizeChange(width, height);
}


IAsyncAction^ Direct3DInterop::OnSuspending()
{
    return m_renderer->OnSuspending();
}

void Direct3DInterop::OnBackKeyPress()
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::shared_ptr<BackButtonEvent> e(new BackButtonEvent());
    mInputEvents.push(e);
}

// Pointer Event Handlers. We need to queue up pointer events to pass them to the drawing thread
void Direct3DInterop::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
    AddPointerEvent(PointerEventType::PointerPressed, args);
}


void Direct3DInterop::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
    AddPointerEvent(PointerEventType::PointerMoved, args);
}

void Direct3DInterop::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
    AddPointerEvent(PointerEventType::PointerReleased, args);
}

void Direct3DInterop::OnCocos2dKeyEvent(Cocos2dKeyEvent key)
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::shared_ptr<KeyboardEvent> e(new KeyboardEvent(key));
    mInputEvents.push(e);
}


void Direct3DInterop::OnCocos2dKeyEvent(Cocos2dKeyEvent key, Platform::String^ text)
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::shared_ptr<KeyboardEvent> e(new KeyboardEvent(key,text));
    mInputEvents.push(e);
}


void Direct3DInterop::AddPointerEvent(PointerEventType type, PointerEventArgs^ args)
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::shared_ptr<PointerEvent> e(new PointerEvent(type, args));
    mInputEvents.push(e);
}

void Direct3DInterop::OnCocos2dEditboxEvent(Object^ sender, Platform::String^ args, Windows::Foundation::EventHandler<Platform::String^>^ handler)
{
	std::lock_guard<std::mutex> guard(mMutex);
	//std::shared_ptr<EditBoxEvent> e(new EditBoxEvent(sender, args, handler));//EDITBOX
	//mInputEvents.push(e);//EDITBOX
}

void Direct3DInterop::ProcessEvents()
{
    std::lock_guard<std::mutex> guard(mMutex);

    while(!mInputEvents.empty())
    {
        InputEvent* e = mInputEvents.front().get();
        e->execute(m_renderer);
        mInputEvents.pop();
    }
}


// Interface With Direct3DContentProvider
void Direct3DInterop::Connect()
{

    m_renderer->Connect();
}

void Direct3DInterop::Disconnect()
{
    m_renderer->Disconnect();
}

void Direct3DInterop::PrepareResources(LARGE_INTEGER presentTargetTime)
{
}

void Direct3DInterop::Draw(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView)
{
 

    m_renderer->UpdateDevice(device, context, renderTargetView);
    if(mCurrentOrientation != WindowOrientation)
    {
        mCurrentOrientation = WindowOrientation;
        m_renderer->OnOrientationChanged(mCurrentOrientation);
    }
    ProcessEvents();
    m_renderer->Render();
}

void Direct3DInterop::SetCocos2dEventDelegate(Cocos2dEventDelegate^ delegate) 
{ 
    m_delegate = delegate; 
    m_renderer->SetXamlEventDelegate(delegate);
}

void Direct3DInterop::SetCocos2dMessageBoxDelegate(Cocos2dMessageBoxDelegate ^ delegate)
{
    m_messageBoxDelegate = delegate;
    m_renderer->SetXamlMessageBoxDelegate(delegate);
}

void Direct3DInterop::SetCocos2dEditBoxDelegate(Cocos2dEditBoxDelegate ^ delegate)
{
    m_editBoxDelegate = delegate;
    m_renderer->SetXamlEditBoxDelegate(delegate);
}


bool Direct3DInterop::SendCocos2dEvent(Cocos2dEvent event)
{
    if(m_delegate)
    {
        m_delegate->Invoke(event);
        return true;
    }
    return false;
}

}