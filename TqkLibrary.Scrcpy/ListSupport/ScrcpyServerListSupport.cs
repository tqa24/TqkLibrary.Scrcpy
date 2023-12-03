using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using TqkLibrary.Scrcpy.Attributes;
using TqkLibrary.Scrcpy.Interfaces;

namespace TqkLibrary.Scrcpy.ListSupport
{
    /// <summary>
    /// 
    /// </summary>
    public class ScrcpyServerListSupport
    {
        static readonly Regex regex_video = new Regex("--video-codec=(\\S+) --video-encoder='(\\S+)'");
        static readonly Regex regex_audio = new Regex("--audio-codec=(\\S+) --audio-encoder='(\\S+)'");
        static readonly Regex regex_display = new Regex("--display=(\\S+) +\\((\\S+)\\)");
        internal static ScrcpyServerListSupport Parse(string data)
        {
            /*
[server] INFO: List of video encoders:
    --video-codec=h264 --video-encoder='OMX.qcom.video.encoder.avc'
    --video-codec=h264 --video-encoder='c2.android.avc.encoder'
    --video-codec=h264 --video-encoder='OMX.google.h264.encoder'
    --video-codec=h265 --video-encoder='OMX.qcom.video.encoder.hevc'
    --video-codec=h265 --video-encoder='OMX.qcom.video.encoder.hevc.cq'
    --video-codec=h265 --video-encoder='c2.android.hevc.encoder'
[server] INFO: List of audio encoders:
    --audio-codec=opus --audio-encoder='c2.android.opus.encoder'
    --audio-codec=aac --audio-encoder='c2.android.aac.encoder'
    --audio-codec=aac --audio-encoder='OMX.google.aac.encoder'
[server] INFO: List of displays:
    --display=0    (1080x2400)


             */

            var datas = data.Split('\r', '\n')
                .Where(x => !string.IsNullOrWhiteSpace(x))
                .Select(x => x.Trim())
                .ToArray();
            ScrcpyServerListSupport result = new ScrcpyServerListSupport();

            foreach (var d in datas)
            {
                Match match = regex_video.Match(d);
                if (match.Success)
                {
                    string codec = match.Groups[1].Value;
                    string encoder = match.Groups[2].Value;
                    result.Videos.Add(new CodecInfo()
                    {
                        Codec = codec,
                        Encoder = encoder,
                    });
                    continue;
                }

                match = regex_audio.Match(d);
                if (match.Success)
                {
                    string codec = match.Groups[1].Value;
                    string encoder = match.Groups[2].Value;
                    result.Audios.Add(new CodecInfo()
                    {
                        Codec = codec,
                        Encoder = encoder,
                    });
                    continue;
                }

                match = regex_display.Match(d);
                if (match.Success)
                {
                    string display = match.Groups[1].Value;
                    string size = match.Groups[2].Value;
                    result.Displays.Add(new DisplayInfo()
                    {
                        Display = display,
                        Size = size,
                    });
                    continue;
                }
            }

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        public List<CodecInfo> Videos { get; set; } = new List<CodecInfo>();
        /// <summary>
        /// 
        /// </summary>
        public List<CodecInfo> Audios { get; set; } = new List<CodecInfo>();
        /// <summary>
        /// 
        /// </summary>
        public List<DisplayInfo> Displays { get; set; } = new List<DisplayInfo>();
    }
}
