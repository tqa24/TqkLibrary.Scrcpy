using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TqkLibrary.Scrcpy.Control
{
    internal delegate void NativeOnClipboardReceivedDelegate(IntPtr intPtr, int length);
    internal class ScrcpyControl : IControl
    {
        internal ScrcpyControl(Scrcpy scrcpy)
        {
            this.Scrcpy = scrcpy;
            this.NativeOnClipboardReceivedDelegate = NativeOnClipboardReceived;
            this.Scrcpy.RegisterClipboardEvent(NativeOnClipboardReceivedDelegate);
        }
        public Scrcpy Scrcpy { get; }

        bool SendControl(ScrcpyControlMessage scrcpyControlMessage) => this.Scrcpy.SendControl(scrcpyControlMessage);

        #region BasicCommand
        public bool BackOrScreenOn(AndroidKeyEventAction KeyEventAction)
            => SendControl(ScrcpyControlMessage.BackOrScreenOn(KeyEventAction));
        public bool CollapsePanel()
            => SendControl(ScrcpyControlMessage.CollapsePanel());
        public bool ExpandNotificationPanel()
            => SendControl(ScrcpyControlMessage.ExpandNotificationPanel());
        public bool ExpandSettingsPanel()
            => SendControl(ScrcpyControlMessage.ExpandSettingsPanel());
        public bool GetClipboard()
            => SendControl(ScrcpyControlMessage.GetClipboard());
        public bool InjectKeycode(
            AndroidKeyEventAction action,
            AndroidKeyCode keycode,
            uint repeat = 1,
            AndroidKeyEventMeta metaState = AndroidKeyEventMeta.META_NONE)
            => SendControl(ScrcpyControlMessage.CreateInjectKeycode(action, keycode, repeat, metaState));
        public bool InjectScrollEvent(Rectangle position, int vScroll, int hScroll = 0)
            => SendControl(ScrcpyControlMessage.CreateInjectScrollEvent(position, vScroll, hScroll));
        public bool InjectText(string text)
            => SendControl(ScrcpyControlMessage.CreateInjectText(text));
        public bool InjectTouchEvent(AndroidMotionEventAction action, long pointerId, Rectangle position, float pressure = 1, AndroidMotionEventButton buttons = AndroidMotionEventButton.BUTTON_PRIMARY)
            => SendControl(ScrcpyControlMessage.CreateInjectTouchEvent(action, pointerId, position, pressure, buttons));
        public bool RotateDevice()
            => SendControl(ScrcpyControlMessage.RotateDevice());
        public bool SetClipboard(string text, bool paste)
            => SendControl(ScrcpyControlMessage.CreateSetClipboard(text, paste));
        public bool SetScreenPowerMode(ScrcpyScreenPowerMode powerMode)
            => SendControl(ScrcpyControlMessage.CreateSetScreenPowerMode(powerMode));
        #endregion

        #region Event
        public event OnDataReceived<string> OnClipboardReceived;
        #endregion

        #region Native Event
        //hold for gc not release this delegate
        readonly NativeOnClipboardReceivedDelegate NativeOnClipboardReceivedDelegate;
        void NativeOnClipboardReceived(IntPtr intPtr, int length)
        {
            if (length == 0)
            {
                ThreadPool.QueueUserWorkItem((o) =>//use thread pool for not hold native thread
                {
                    OnClipboardReceived?.Invoke(string.Empty);
                });
            }
            else
            {
                byte[] buffer = new byte[length];
                Marshal.Copy(intPtr, buffer, 0, length);
                ThreadPool.QueueUserWorkItem((o) =>//use thread pool for not hold native thread
                {
                    OnClipboardReceived?.Invoke(Encoding.UTF8.GetString(buffer));
                });
            }
        }
        #endregion
    }
}
