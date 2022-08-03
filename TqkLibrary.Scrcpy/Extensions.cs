using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TqkLibrary.Scrcpy
{
    internal static class Extensions
    {
        public static bool SafeTryAddCount(this CountdownEvent countdownEvent)
        {
            try
            {
                return countdownEvent.TryAddCount();
            }
            catch(ObjectDisposedException)
            {
                return false;
            }
        }
    }
}
