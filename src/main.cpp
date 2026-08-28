#define UNICODE

#include <windows.h>
#include <wrl.h>
#include <WebView2.h>

using namespace Microsoft::WRL;

// ------------------------------------------------------------
// Global WebView2 objects
// ------------------------------------------------------------

ComPtr<ICoreWebView2Controller> webViewController;
ComPtr<ICoreWebView2> webView;

// ------------------------------------------------------------
// Forward declarations
// ------------------------------------------------------------

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

void InitializeWebView(HWND hwnd);
void ResizeWebView(HWND hwnd);

// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    // These aren't currently used.
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    // --------------------------------------------------------
    // Register the window class.
    // --------------------------------------------------------

    const wchar_t CLASS_NAME[] = L"Backdrop Window Class";

    WNDCLASSW wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon   = LoadIcon(hInstance, MAKEINTRESOURCE(1)); 

    RegisterClassW(&wc);

    // --------------------------------------------------------
    // Get screen dimensions.
    // --------------------------------------------------------

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // --------------------------------------------------------
    // Create the window.
    // --------------------------------------------------------

    HWND hwnd = CreateWindowW(
        CLASS_NAME,
        L"Backdrop",
        WS_POPUP | WS_VISIBLE,
        0,
        0,
        screenWidth,
        screenHeight,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // --------------------------------------------------------
    // Initialize WebView2.
    // --------------------------------------------------------

    InitializeWebView(hwnd);

    // --------------------------------------------------------
    // Run the message loop.
    // --------------------------------------------------------

    MSG msg = {};

    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

// ------------------------------------------------------------
// Initialize WebView2
// ------------------------------------------------------------

void InitializeWebView(HWND hwnd)
{
    HRESULT result = CreateCoreWebView2EnvironmentWithOptions(
        nullptr,
        nullptr,
        nullptr,

        Callback<
            ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler
        >(
            [hwnd](
                HRESULT result,
                ICoreWebView2Environment* environment
            ) -> HRESULT
            {
                if (FAILED(result))
                {
                    MessageBoxW(
                        hwnd,
                        L"Failed to create the WebView2 environment.",
                        L"Backdrop",
                        MB_OK | MB_ICONERROR
                    );

                    return result;
                }

                // ------------------------------------------------
                // Create the WebView2 controller.
                // ------------------------------------------------

                return environment->CreateCoreWebView2Controller(
                    hwnd,

                    Callback<
                        ICoreWebView2CreateCoreWebView2ControllerCompletedHandler
                    >(
                        [hwnd](
                            HRESULT result,
                            ICoreWebView2Controller* controller
                        ) -> HRESULT
                        {
                            if (FAILED(result))
                            {
                                MessageBoxW(
                                    hwnd,
                                    L"Failed to create the WebView2 controller.",
                                    L"Backdrop",
                                    MB_OK | MB_ICONERROR
                                );

                                return result;
                            }

                            // Store the controller.
                            webViewController = controller;

                            // Get the actual WebView.
                            HRESULT getWebViewResult =
                                controller->get_CoreWebView2(
                                    webView.GetAddressOf()
                                );

                            if (FAILED(getWebViewResult))
                            {
                                return getWebViewResult;
                            }

                            // Make WebView2 fill the entire window.
                            ResizeWebView(hwnd);

                            // ------------------------------------------------
                            // Load our HTML page.
                            //
                            // This path is temporary. We'll make it
                            // automatically locate the web folder later.
                            // ------------------------------------------------

                            webView->Navigate(
                                L"file:///C:/Users/ethma/Documents/Ethan_work/home/Ethan_codes/Backdrop/src/web/index.html"
                            );

                            return S_OK;
                        }
                    ).Get()
                );
            }
        ).Get()
    );

    if (FAILED(result))
    {
        MessageBoxW(
            hwnd,
            L"Failed to initialize WebView2.",
            L"Backdrop",
            MB_OK | MB_ICONERROR
        );
    }
}

// ------------------------------------------------------------
// Resize WebView2 to fill the window.
// ------------------------------------------------------------

void ResizeWebView(HWND hwnd)
{
    if (webViewController == nullptr)
    {
        return;
    }

    RECT bounds;

    GetClientRect(hwnd, &bounds);

    webViewController->put_Bounds(bounds);
}

// ------------------------------------------------------------
// Window procedure
// ------------------------------------------------------------

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
    // --------------------------------------------------------
    // Temporary solution to close window.
    // --------------------------------------------------------

    case WM_KEYDOWN:

        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }

        return 0;

    // --------------------------------------------------------
    // Resize WebView2 whenever the window changes size.
    // --------------------------------------------------------

    case WM_SIZE:

        ResizeWebView(hwnd);

        return 0;

    // --------------------------------------------------------
    // Window is being destroyed.
    // --------------------------------------------------------

    case WM_DESTROY:

        webView.Reset();
        webViewController.Reset();

        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProcW(
        hwnd,
        uMsg,
        wParam,
        lParam
    );
}