
#include "Direct3DContentProvider.h"

using namespace PhoneDirect3DXamlAppComponent;

Direct3DContentProvider::Direct3DContentProvider(Direct3DInterop^ controller) :
    m_controller(controller)
{
}

// IDrawingSurfaceContentProviderNative interface
HRESULT Direct3DContentProvider::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
{
    m_host = host;
    Initialize();
    m_controller->Connect();

    return S_OK;
}

void Direct3DContentProvider::Disconnect()
{
    m_controller->Disconnect();
    m_host = nullptr;
    m_synchronizedTexture = nullptr;
    m_d3dDevice = nullptr;
    m_d3dContext = nullptr;
    m_renderTarget = nullptr;
    m_renderTargetView = nullptr;
}

HRESULT Direct3DContentProvider::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
{
    m_controller->PrepareResources(*presentTargetTime);
    *contentDirty = true;

    return S_OK;
}

HRESULT Direct3DContentProvider::GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle)
{
    if (!size || !synchronizedTexture || !textureSubRectangle) return E_INVALIDARG;

    HRESULT hr = S_OK;

    if (!m_synchronizedTexture ||
        m_renderTargetSize.Width != m_controller->WindowBounds.Width ||
        m_renderTargetSize.Height != m_controller->WindowBounds.Height)
    {
        UpdateForWindowSizeChange(m_controller->WindowBounds.Width, m_controller->WindowBounds.Height);
        hr = InitializeTexture();
    }

    // Draw to the texture.
    if (SUCCEEDED(hr))
    {
        hr = m_synchronizedTexture->BeginDraw();
        
        if (SUCCEEDED(hr))
        {
            m_controller->Draw(m_d3dDevice.Get(), m_d3dContext.Get(), m_renderTargetView.Get());
            hr = m_synchronizedTexture->EndDraw();
        }
    }

    // Set output parameters.
    if (SUCCEEDED(hr))
    {    
        textureSubRectangle->left = 0.0f;
        textureSubRectangle->top = 0.0f;
	    textureSubRectangle->right = static_cast<FLOAT>(size->width);
	    textureSubRectangle->bottom = static_cast<FLOAT>(size->height);

        hr = m_synchronizedTexture.CopyTo(synchronizedTexture);

        if (SUCCEEDED(hr))
        {
            hr = m_host->RequestAdditionalFrame();
        }
    }

    return hr;
}

HRESULT Direct3DContentProvider::InitializeTexture()
{
    return m_host->CreateSynchronizedTexture(m_renderTarget.Get(), &m_synchronizedTexture);
}
