using Key4WiFi.Tools;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.IO.Compression;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;


// 安装 Newtonsoft.Json 引用

namespace Key4WiFi
{

    public class WiFiMasterKey
    {


        static void Main(string[] args)
        {
            WiFiMasterKey obj = new WiFiMasterKey();
            obj.GetPassword("HAHAHAHA", "24:69:68:6F:E2:A0");


        }

        string _dhid;

        string qid;

        public string dhid
        {
            get
            {
                return _dhid;
            }
        }

        public WiFiMasterKey()
        {
            _dhid = GETdhid();
        }

        public static  string GETdhid()
        {
            HttpWebRequest webrequest = (HttpWebRequest)WebRequest.Create("http://sso.51y5.net/sso/fa.sec");
            webrequest.ContentType = "application/x-www-form-urlencoded";
            webrequest.Method = "POST";

            webrequest.UserAgent = "Dalvik/1.6.0 (Linux; U; Android 4.4.2; NoxW Build/KOT49H)";

            webrequest.Host = "sso.51y5.net";
            webrequest.KeepAlive = true;
            webrequest.Headers.Add("Accept-Encoding:gzip");
            //webrequest.Headers[""] = "gzip";


            string postHeader = "appId=A0008&ed=27C6B45D6184F0FF3C8375CE448755C7977D974B4C68F239CD6C0D03B5D886F1B17A8BD3A853596987D2BF18A62C0B47F4864391B01B656C13A0E37BCA710EDB1944E1CE68F871541E37210303C62487CE95276839B3E68E0BAE372185417AB65E166CBB0F46410B2123DBE679E5CF34F931F31082F8F42BD0E9EED5D3B613F8A6CEBB28C2ADD94425B4BF19C9FAACD86E4D99C98F6C1468255FA6C3D58070DAA2C511DBEBB8A13A695B12EEB852612AE913615612700C2D5AFC1E93E1CA408E7882055AB55446AC27C3BD8A6882F462EAAEF70F04AC39BD717BEDFF6EE0C7F9C905F81EC09A1D77F755CF062EC395934FB0369A8EE76B7304F4EF0BAE8BC3C24E13C842F07663460270A898F6DD9E34179D513E2ADFE43DD8AEA68131103256479E00D1861FAEDFB3105A6168C1437F3A3675BA40E567B278C0DC8EA7220F2F82AF853CB5AC5FA8C7CAB49F198552996413AAAA3D3173C5180558A08F822DAFC4344FAFA9FFA327C1F149244C9BAACA451BD591A4D7E6B5E0C7466B302DBF66B26553D3D1403F6E460B110FF41DA7E88E2C9C538DC44DDCD645C934C23BD6539AF556D720CB7DEF1ABA1B8F893E29682F2CEE64BFA6C1B7DFEB3549BEB6A544F3B384722558F1FDDC12ADA31F0E337C0FD772807AF362CA4AE68A02354892D07A9D1E2BA20D91D811D1A14DD92B76ADDB69F6261757203E0D3E1209D9AC78F0F700AC738B08830B0ACCE71B2B9BC8BBCB997458B87E892EAA5F327D1478586EB8DDE43DF31CCF07CE145520104C25E8D2333033742B8C05A768EAA4C4EB7EAE29923D72ED78631CDBE356D2F3EB1C185F3621EB53D014F144CED2D5C60AAE1C1D09A98F7C356B887C1CE2873436FE6E67A64BFE8D4C8F7DF97B49AD89AE6C36AF3DA2CE62163E4B4AEB5DBC9F69DB07FDCF76DBB97F1DB3DF477A5213011099A07A616883498C8C1331E0632BF0C8616257796942722A87B97E8A0C2D8F92896AD29786900EE593C7754F14A9325B780A1652D11A2ED784085A62FD88CFF4895E771ADA4E7EF81042298FCF949ABD052DDCE7394AE9C8931472D14F40E75A814E813A2FC77D92F6FC3BE3C391E9E5C49EA1466A21E31BA2A6C065422DFA2988B564820D1B06D3BA25C6695D397DE98A8EAD36A0EDAA8E4088AF1C1F56F00D63B0926E53D7C086A7FE974A706D739E4AF73F37D6A50703C87F0260ED8574B716055FD1BF2BD81F49F86C765FA90AFA060B8D91A7A9F9ED6B3EBB061529B403438471215A3746B64045DFB1CE5C01F67557494BCFD7EC279F387DD5C252F610568B32BF597A08185696350405CEAAD591AA076178ACB8E68B7CF0EB3F23447786C9EDC73E427F66671C9A814B1186B5DBC921472160A71FE79E983446C671FD690C820B3A7C8188DF4394A819AD7B1320338C105AEE3C4B4A950C671C303FAA05FC2D371A297F1C5AE9D70FF4D72DD87A&st=m&pid=00200201&sign=689C5D2BA2EE99FB605CC0438CF9975D&et=a";
            byte[] postHeaderBytes = Encoding.UTF8.GetBytes(postHeader);


            long length = postHeaderBytes.Length;

            Stream requestStream =  webrequest.GetRequestStream();


            requestStream.Write(postHeaderBytes, 0, postHeaderBytes.Length);

            WebResponse responce =  webrequest.GetResponse();
            Stream s = responce.GetResponseStream();
            StreamReader sr = new StreamReader(s);


            string tmp = sr.ReadToEnd();

            JObject jo = JObject.Parse(tmp);


            JToken jt;

            Tools.return_dhid rd = new Tools.return_dhid();
            rd.RawString = tmp;


            if (((string)jo["retCd"]) != "0")
            {
                rd.has = false;
            }
            else if (jo.TryGetValue("dhid", out jt))
            {
                rd.has = true;
                rd.dhid = jt.ToString();
            }
            else
            {
                rd.has = false;
            }

            return rd.dhid;
        }

        public async Task<return_pwd> ScanWiFi(string sb)
        {
            Dictionary<string, string> data = new Dictionary<string, string>();


            data.Add("origChanId", "gw");//chanid
            data.Add("sn", "46001");
            data.Add("appId", "A0008");//appid
            data.Add("ts", Time.UnixTime().ToString());
            data.Add("netModel", "w");
            data.Add("chanId", "360");//chanid
            data.Add("imei", "357541051318147");
            data.Add("mac", "e8:92:a2:9b:16:43");//本机mac
            data.Add("capSsid", "hiJack");//已连接的WiFi SSID，影响不大，留空
            data.Add("lang", "cn");//lang
            data.Add("longi", "0.0");//精度
            data.Add("capBssid", "b0:d3:9d:45:b9:85");//已连接的WiFi BSSID，影响不大，留空
            data.Add("mapSP", "a");
            data.Add("userToken", "");
            data.Add("sb", sb);
            data.Add("verName", "4.1.32");
            data.Add("verCode", "3062");
            data.Add("uhid", "a0000000000000000000000000000001");
            data.Add("lati", "0.0");//纬度
            data.Add("dhid", _dhid);//dhid

            SortedDictionary<string, string> sd = new SortedDictionary<string, string>(data);

            string dt = JsonConvert.SerializeObject(sd);



            byte[] cd = GZip.CompressionData(Encoding.UTF8.GetBytes(dt));//对字符串进行Gzip压缩

            //将byte长度补足至16的倍数，否则会导致加密时块不完整而报错
            int len = cd.Length;
            byte[] b;
            if (len % 16 != 0)
            {
                len = cd.Length;
                len /= 16;
                len++;//++len
                len *= 16;
                b = new byte[len];
                Array.Copy(cd, 0, b, 0, cd.Length);
            }
            else
            {
                b = cd;
            }

            string ed = Convert.ToBase64String(AesHelper.Encrypt(b, "!I50#LSSciCx&q6E", "$t%s%12#2b474pXF"));



            //string tmp = AESDecrypt(dt, "!I50#LSSciCx&q6E", "t%s%12#2b474pXF");

            data = new Dictionary<string, string>();
            data.Add("appId", "A0008");
            data.Add("pid", "00300108");
            //data.Add("pid", "00300108:00300111:00300110:00300901");
            data.Add("ed", ed);
            data.Add("st", "m");
            data.Add("et", "a");


            //使用固有密钥进行签名
            string Sign = sign(data, "*Lm%qiOHVEedH3%A^uFFsZvFH9T8QAZe");

            data.Add("sign", Sign);
            data.Add("pv", "1.0");



            HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://ap.51y5.net/ap/fa.sec");
            request.Headers["Accept-Encoding"] = "gzip";
            //request.Headers["User-Agent"] = "Dalvik/1.6.0 (Linux; U; Android 4.2.2; GT-I9082 Build/JDQ39E)";
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";

            //通过Dictionary初始化请求字符
            string request_str = "";
            List<string> keys = new List<string>(data.Keys);
            List<string> value = new List<string>(data.Values);
            for (int i = 0; i < data.Count; i++)
            {
                request_str += keys[i] + "=" + WebUtility.UrlEncode(value[i]) + "&";
            }
            request_str.Remove(request_str.Length - 2);
            //request_str = WebUtility.UrlEncode(request_str);


            byte[] b2 = Encoding.UTF8.GetBytes(request_str);
            Stream stream = await request.GetRequestStreamAsync();
            await stream.WriteAsync(b2, 0, b2.Length);
            //stream.Dispose();
            HttpWebResponse response = (HttpWebResponse)await request.GetResponseAsync();
            StreamReader sr = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
            string ret = sr.ReadToEnd();
            stream.Dispose();
            sr.Dispose();
            response.Dispose();


            Match qidmatch = Regex.Match(ret, "\"qid\":\"(.*?)\"");
            qid = qidmatch.Groups[1].Value;
            Match apsmatch = Regex.Match(ret, "\"aps\":\\{(.*?)\\}");
            MatchCollection mc = Regex.Matches(apsmatch.Groups[1].Value, "\"(.*?)\":{\"apRefId\":\"(.*?)\",\"ssid\":\"(.*?)\",\"qt\":\"(.*?)\"");
            return_pwd rp = new return_pwd();
            foreach (Match match in mc)
            {
                rp.Add(new pwd() { SSID = match.Groups[3].Value, BSSID = match.Groups[1].Value });
            }
            return rp;

        }


        public string GetPassword(string ssid, string bssid)
        {
            Dictionary<string, string> data = new Dictionary<string, string>();


            data.Add("origChanId", "gw");//chanid
            data.Add("sn", "46001");
            data.Add("appId", "A0008");//appid
            data.Add("ts", Time.UnixTime().ToString());
            data.Add("netModel", "w");
            data.Add("imei", "357541051318147");
            data.Add("chanId", "360");//chanid
            data.Add("qid", "0a83063e46a05d2d92410a007ea1");
            data.Add("capSsid", "hiJack");
            data.Add("mac", "e8:92:a2:9b:16:43");//mac
            data.Add("lang", "cn");//lang
            data.Add("nbaps", "");
            data.Add("longi", "0.0");

            data.Add("bssid", bssid);

            data.Add("capBssid", "b0:d3:9d:45:b9:85");
            data.Add("mapSP", "a");
            data.Add("userToken", "");
            data.Add("ssid", ssid);
            data.Add("verName", "4.1.32");

            data.Add("uhid", "a0000000000000000000000000000001");
            data.Add("verCode", "3062");
            data.Add("lati", "0.0");
            data.Add("dhid", dhid);//dhid
            SortedDictionary<string, string> sd = new SortedDictionary<string, string>(data);

            string dt = JsonConvert.SerializeObject(sd);



            byte[] cd = GZip.CompressionData(Encoding.UTF8.GetBytes(dt));
            int len = cd.Length;
            byte[] b;

            if (len % 16 != 0)
            {
                len = cd.Length;
                len /= 16;
                len++;//++len
                len *= 16;
                b = new byte[len];
                Array.Copy(cd, 0, b, 0, cd.Length);
            }
            else
            {
                b = cd;
            }

            string ed = Convert.ToBase64String(AesHelper.Encrypt(b, "!I50#LSSciCx&q6E", "$t%s%12#2b474pXF"));




            //string tmp = AESDecrypt(dt, "!I50#LSSciCx&q6E", "t%s%12#2b474pXF");

            data = new Dictionary<string, string>();
            data.Add("appId", "A0008");
            data.Add("pid", "00300109");
            //data.Add("pid", "00300108:00300111:00300110:00300901");
            data.Add("ed", ed);
            data.Add("st", "m");
            data.Add("et", "a");

            //data.Add("dhid", dhid);
            //data.Add("verCode", "3028");
            //data.Add("sv", "1.0");


            string Sign = sign(data, "*Lm%qiOHVEedH3%A^uFFsZvFH9T8QAZe");

            data.Add("sign", Sign);

            data.Add("pv", "1.0");



            HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://ap.51y5.net/ap/fa.sec");
            request.Headers["Accept-Encoding"] = "gzip";
            //request.Headers["User-Agent"] = "Dalvik/1.6.0 (Linux; U; Android 4.2.2; GT-I9082 Build/JDQ39E)";
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";


            string request_str = "";
            List<string> keys = new List<string>(data.Keys);
            List<string> value = new List<string>(data.Values);
            for (int i = 0; i < data.Count; i++)
            {
                request_str += keys[i] + "=" + WebUtility.UrlEncode(value[i]) + "&";
            }
            request_str.Remove(request_str.Length - 2);
            //request_str = WebUtility.UrlEncode(request_str);


            byte[] b2 = Encoding.UTF8.GetBytes(request_str);
            Stream stream = request.GetRequestStream();
            
            stream.Write(b2, 0, b2.Length);
            //stream.Dispose();
            HttpWebResponse response = (HttpWebResponse) request.GetResponse();
            StreamReader sr = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
            string ret = sr.ReadToEnd();//"{\"retCd\":\"H.RISK.0001\",\"retMsg\":\"请点击右下角\\\"我的\\\"-\\\"设置\\\"-\\\"版本更新\\\"，更新最新版本后，再次尝试使用免费连接功能。或到官网https://www.wifi.com下载最新版本。\",\"qid\":\"0a83063e46a05d2d92410a007ea1\",\"sysTime\":\"1563267649679\"}"
            stream.Dispose();
            sr.Dispose();
            response.Dispose();


            Match m = Regex.Match(ret, ",\"pwd\":\"(.*?)\",\"ssid\"");
            byte[] by = Tools.StringHelper.stringToByteArray(m.Groups[1].Value);
            string tmp = Encoding.UTF8.GetString(AesHelper.Decrypt(by, "!I50#LSSciCx&q6E", "$t%s%12#2b474pXF"));
            len = Convert.ToInt32(tmp.Remove(3, tmp.Length - 3));
            tmp = tmp.Substring(3, len);
            return tmp;

        }

        private static string sign(Dictionary<string, string> array, string salt)
        {
            string request_str = "";

            //对输入的Dictionary进行整理
            List<string> list = new List<string>();
            SortedDictionary<string, string> a = new SortedDictionary<string, string>(array);
            foreach (KeyValuePair<string, string> kvp in a)
            {
                list.Add(kvp.Value);
            }


            foreach (string str in list)
            {
                request_str += str;
            }
            string sign = MD5.GetMd5String(request_str + salt);
            return sign.ToUpperInvariant();
        }


    }

    class MD5 : IDisposable
    {
        private const byte S11 = 7;

        private const byte S12 = 12;

        private const byte S13 = 17;

        private const byte S14 = 22;

        private const byte S21 = 5;

        private const byte S22 = 9;

        private const byte S23 = 14;

        private const byte S24 = 20;

        private const byte S31 = 4;

        private const byte S32 = 11;

        private const byte S33 = 16;

        private const byte S34 = 23;

        private const byte S41 = 6;

        private const byte S42 = 10;

        private const byte S43 = 15;

        private const byte S44 = 21;

        private static byte[] PADDING;

        private uint[] state = new uint[4];

        private uint[] count = new uint[2];

        private byte[] buffer = new byte[64];

        protected byte[] HashValue;

        protected int State;

        protected int HashSizeValue = 128;

        public virtual bool CanReuseTransform
        {
            get
            {
                return true;
            }
        }

        public virtual bool CanTransformMultipleBlocks
        {
            get
            {
                return true;
            }
        }

        public virtual byte[] Hash
        {
            get
            {
                if (this.State != 0)
                {
                    throw new InvalidOperationException();
                }
                return (byte[])this.HashValue.Clone();
            }
        }

        public virtual int HashSize
        {
            get
            {
                return this.HashSizeValue;
            }
        }

        public virtual int InputBlockSize
        {
            get
            {
                return 1;
            }
        }

        public virtual int OutputBlockSize
        {
            get
            {
                return 1;
            }
        }

        public static MD5 Create(string hashName)
        {
            if (hashName == "MD5")
            {
                return new MD5();
            }
            throw new NotSupportedException();
        }

        public static string GetMd5String(string source)
        {
            MD5 mD = MD5.Create();
            UTF8Encoding uTF8Encoding = new UTF8Encoding();
            byte[] bytes = uTF8Encoding.GetBytes(source);
            byte[] array = mD.ComputeHash(bytes);
            StringBuilder stringBuilder = new StringBuilder();
            byte[] array2 = array;
            for (int i = 0; i < array2.Length; i++)
            {
                byte b = array2[i];
                stringBuilder.Append(b.ToString("x2"));
            }
            return stringBuilder.ToString();
        }

        public static string GetMd5String(byte[] source)
        {
            MD5 mD = MD5.Create();
            byte[] array = mD.ComputeHash(source);
            StringBuilder stringBuilder = new StringBuilder();
            byte[] array2 = array;
            for (int i = 0; i < array2.Length; i++)
            {
                byte b = array2[i];
                stringBuilder.Append(b.ToString("x2"));
            }
            return stringBuilder.ToString();
        }

        public static byte[] GetMd5ByteArray(string source)
        {
            MD5 mD = MD5.Create();
            UTF8Encoding uTF8Encoding = new UTF8Encoding();
            byte[] bytes = uTF8Encoding.GetBytes(source);
            return mD.ComputeHash(bytes);
        }

        public static MD5 Create()
        {
            return new MD5();
        }

        private static uint F(uint x, uint y, uint z)
        {
            return (x & y) | (~x & z);
        }

        private static uint G(uint x, uint y, uint z)
        {
            return (x & z) | (y & ~z);
        }

        private static uint H(uint x, uint y, uint z)
        {
            return x ^ y ^ z;
        }

        private static uint I(uint x, uint y, uint z)
        {
            return y ^ (x | ~z);
        }

        private static uint ROTATE_LEFT(uint x, byte n)
        {
            return x << (int)n | x >> (int)(32 - n);
        }

        private static void FF(ref uint a, uint b, uint c, uint d, uint x, byte s, uint ac)
        {
            a += MD5.F(b, c, d) + x + ac;
            a = MD5.ROTATE_LEFT(a, s);
            a += b;
        }

        private static void GG(ref uint a, uint b, uint c, uint d, uint x, byte s, uint ac)
        {
            a += MD5.G(b, c, d) + x + ac;
            a = MD5.ROTATE_LEFT(a, s);
            a += b;
        }

        private static void HH(ref uint a, uint b, uint c, uint d, uint x, byte s, uint ac)
        {
            a += MD5.H(b, c, d) + x + ac;
            a = MD5.ROTATE_LEFT(a, s);
            a += b;
        }

        private static void II(ref uint a, uint b, uint c, uint d, uint x, byte s, uint ac)
        {
            a += MD5.I(b, c, d) + x + ac;
            a = MD5.ROTATE_LEFT(a, s);
            a += b;
        }

        internal MD5()
        {
            this.Initialize();
        }

        public virtual void Initialize()
        {
            this.count[0] = (this.count[1] = 0u);
            this.state[0] = 1732584193u;
            this.state[1] = 4023233417u;
            this.state[2] = 2562383102u;
            this.state[3] = 271733878u;
        }

        protected virtual void HashCore(byte[] input, int offset, int count)
        {
            int num = (int)(this.count[0] >> 3 & 63u);
            if ((this.count[0] += (uint)((uint)count << 3)) < (uint)((uint)count << 3))
            {
                this.count[1] += 1u;
            }
            this.count[1] += (uint)count >> 29;
            int num2 = 64 - num;
            int num3;
            if (count >= num2)
            {
                System.Buffer.BlockCopy(input, offset, this.buffer, num, num2);
                this.Transform(this.buffer, 0);
                num3 = num2;
                while (num3 + 63 < count)
                {
                    this.Transform(input, offset + num3);
                    num3 += 64;
                }
                num = 0;
            }
            else
            {
                num3 = 0;
            }
            System.Buffer.BlockCopy(input, offset + num3, this.buffer, num, count - num3);
        }

        protected virtual byte[] HashFinal()
        {
            byte[] array = new byte[16];
            byte[] array2 = new byte[8];
            MD5.Encode(array2, 0, this.count, 0, 8);
            int num = (int)(this.count[0] >> 3 & 63u);
            int num2 = (num < 56) ? (56 - num) : (120 - num);
            this.HashCore(MD5.PADDING, 0, num2);
            this.HashCore(array2, 0, 8);
            MD5.Encode(array, 0, this.state, 0, 16);
            this.count[0] = (this.count[1] = 0u);
            this.state[0] = 0u;
            this.state[1] = 0u;
            this.state[2] = 0u;
            this.state[3] = 0u;
            this.Initialize();
            return array;
        }

        private void Transform(byte[] block, int offset)
        {
            uint num = this.state[0];
            uint num2 = this.state[1];
            uint num3 = this.state[2];
            uint num4 = this.state[3];
            uint[] array = new uint[16];
            MD5.Decode(array, 0, block, offset, 64);
            MD5.FF(ref num, num2, num3, num4, array[0], 7, 3614090360u);
            MD5.FF(ref num4, num, num2, num3, array[1], 12, 3905402710u);
            MD5.FF(ref num3, num4, num, num2, array[2], 17, 606105819u);
            MD5.FF(ref num2, num3, num4, num, array[3], 22, 3250441966u);
            MD5.FF(ref num, num2, num3, num4, array[4], 7, 4118548399u);
            MD5.FF(ref num4, num, num2, num3, array[5], 12, 1200080426u);
            MD5.FF(ref num3, num4, num, num2, array[6], 17, 2821735955u);
            MD5.FF(ref num2, num3, num4, num, array[7], 22, 4249261313u);
            MD5.FF(ref num, num2, num3, num4, array[8], 7, 1770035416u);
            MD5.FF(ref num4, num, num2, num3, array[9], 12, 2336552879u);
            MD5.FF(ref num3, num4, num, num2, array[10], 17, 4294925233u);
            MD5.FF(ref num2, num3, num4, num, array[11], 22, 2304563134u);
            MD5.FF(ref num, num2, num3, num4, array[12], 7, 1804603682u);
            MD5.FF(ref num4, num, num2, num3, array[13], 12, 4254626195u);
            MD5.FF(ref num3, num4, num, num2, array[14], 17, 2792965006u);
            MD5.FF(ref num2, num3, num4, num, array[15], 22, 1236535329u);
            MD5.GG(ref num, num2, num3, num4, array[1], 5, 4129170786u);
            MD5.GG(ref num4, num, num2, num3, array[6], 9, 3225465664u);
            MD5.GG(ref num3, num4, num, num2, array[11], 14, 643717713u);
            MD5.GG(ref num2, num3, num4, num, array[0], 20, 3921069994u);
            MD5.GG(ref num, num2, num3, num4, array[5], 5, 3593408605u);
            MD5.GG(ref num4, num, num2, num3, array[10], 9, 38016083u);
            MD5.GG(ref num3, num4, num, num2, array[15], 14, 3634488961u);
            MD5.GG(ref num2, num3, num4, num, array[4], 20, 3889429448u);
            MD5.GG(ref num, num2, num3, num4, array[9], 5, 568446438u);
            MD5.GG(ref num4, num, num2, num3, array[14], 9, 3275163606u);
            MD5.GG(ref num3, num4, num, num2, array[3], 14, 4107603335u);
            MD5.GG(ref num2, num3, num4, num, array[8], 20, 1163531501u);
            MD5.GG(ref num, num2, num3, num4, array[13], 5, 2850285829u);
            MD5.GG(ref num4, num, num2, num3, array[2], 9, 4243563512u);
            MD5.GG(ref num3, num4, num, num2, array[7], 14, 1735328473u);
            MD5.GG(ref num2, num3, num4, num, array[12], 20, 2368359562u);
            MD5.HH(ref num, num2, num3, num4, array[5], 4, 4294588738u);
            MD5.HH(ref num4, num, num2, num3, array[8], 11, 2272392833u);
            MD5.HH(ref num3, num4, num, num2, array[11], 16, 1839030562u);
            MD5.HH(ref num2, num3, num4, num, array[14], 23, 4259657740u);
            MD5.HH(ref num, num2, num3, num4, array[1], 4, 2763975236u);
            MD5.HH(ref num4, num, num2, num3, array[4], 11, 1272893353u);
            MD5.HH(ref num3, num4, num, num2, array[7], 16, 4139469664u);
            MD5.HH(ref num2, num3, num4, num, array[10], 23, 3200236656u);
            MD5.HH(ref num, num2, num3, num4, array[13], 4, 681279174u);
            MD5.HH(ref num4, num, num2, num3, array[0], 11, 3936430074u);
            MD5.HH(ref num3, num4, num, num2, array[3], 16, 3572445317u);
            MD5.HH(ref num2, num3, num4, num, array[6], 23, 76029189u);
            MD5.HH(ref num, num2, num3, num4, array[9], 4, 3654602809u);
            MD5.HH(ref num4, num, num2, num3, array[12], 11, 3873151461u);
            MD5.HH(ref num3, num4, num, num2, array[15], 16, 530742520u);
            MD5.HH(ref num2, num3, num4, num, array[2], 23, 3299628645u);
            MD5.II(ref num, num2, num3, num4, array[0], 6, 4096336452u);
            MD5.II(ref num4, num, num2, num3, array[7], 10, 1126891415u);
            MD5.II(ref num3, num4, num, num2, array[14], 15, 2878612391u);
            MD5.II(ref num2, num3, num4, num, array[5], 21, 4237533241u);
            MD5.II(ref num, num2, num3, num4, array[12], 6, 1700485571u);
            MD5.II(ref num4, num, num2, num3, array[3], 10, 2399980690u);
            MD5.II(ref num3, num4, num, num2, array[10], 15, 4293915773u);
            MD5.II(ref num2, num3, num4, num, array[1], 21, 2240044497u);
            MD5.II(ref num, num2, num3, num4, array[8], 6, 1873313359u);
            MD5.II(ref num4, num, num2, num3, array[15], 10, 4264355552u);
            MD5.II(ref num3, num4, num, num2, array[6], 15, 2734768916u);
            MD5.II(ref num2, num3, num4, num, array[13], 21, 1309151649u);
            MD5.II(ref num, num2, num3, num4, array[4], 6, 4149444226u);
            MD5.II(ref num4, num, num2, num3, array[11], 10, 3174756917u);
            MD5.II(ref num3, num4, num, num2, array[2], 15, 718787259u);
            MD5.II(ref num2, num3, num4, num, array[9], 21, 3951481745u);
            this.state[0] += num;
            this.state[1] += num2;
            this.state[2] += num3;
            this.state[3] += num4;
            for (int i = 0; i < array.Length; i++)
            {
                array[i] = 0u;
            }
        }

        private static void Encode(byte[] output, int outputOffset, uint[] input, int inputOffset, int count)
        {
            int num = outputOffset + count;
            int num2 = inputOffset;
            for (int i = outputOffset; i < num; i += 4)
            {
                output[i] = (byte)(input[num2] & 255u);
                output[i + 1] = (byte)(input[num2] >> 8 & 255u);
                output[i + 2] = (byte)(input[num2] >> 16 & 255u);
                output[i + 3] = (byte)(input[num2] >> 24 & 255u);
                num2++;
            }
        }

        private static void Decode(uint[] output, int outputOffset, byte[] input, int inputOffset, int count)
        {
            int num = inputOffset + count;
            int num2 = outputOffset;
            for (int i = inputOffset; i < num; i += 4)
            {
                output[num2] = (uint)((int)input[i] | (int)input[i + 1] << 8 | (int)input[i + 2] << 16 | (int)input[i + 3] << 24);
                num2++;
            }
        }

        public void Clear()
        {
            this.Dispose(true);
        }

        public byte[] ComputeHash(byte[] buffer)
        {
            return this.ComputeHash(buffer, 0, buffer.Length);
        }

        public byte[] ComputeHash(byte[] buffer, int offset, int count)
        {
            this.Initialize();
            this.HashCore(buffer, offset, count);
            this.HashValue = this.HashFinal();
            return (byte[])this.HashValue.Clone();
        }

        public byte[] ComputeHash(Stream inputStream)
        {
            this.Initialize();
            byte[] array = new byte[4096];
            int num;
            while (0 < (num = inputStream.Read(array, 0, 4096)))
            {
                this.HashCore(array, 0, num);
            }
            this.HashValue = this.HashFinal();
            return (byte[])this.HashValue.Clone();
        }

        public int TransformBlock(byte[] inputBuffer, int inputOffset, int inputCount, byte[] outputBuffer, int outputOffset)
        {
            if (inputBuffer == null)
            {
                throw new ArgumentNullException("inputBuffer");
            }
            if (inputOffset < 0)
            {
                throw new ArgumentOutOfRangeException("inputOffset");
            }
            if (inputCount < 0 || inputCount > inputBuffer.Length)
            {
                throw new ArgumentException("inputCount");
            }
            if (inputBuffer.Length - inputCount < inputOffset)
            {
                throw new ArgumentOutOfRangeException("inputOffset");
            }
            if (this.State == 0)
            {
                this.Initialize();
                this.State = 1;
            }
            this.HashCore(inputBuffer, inputOffset, inputCount);
            if (inputBuffer != outputBuffer || inputOffset != outputOffset)
            {
                System.Buffer.BlockCopy(inputBuffer, inputOffset, outputBuffer, outputOffset, inputCount);
            }
            return inputCount;
        }

        public byte[] TransformFinalBlock(byte[] inputBuffer, int inputOffset, int inputCount)
        {
            if (inputBuffer == null)
            {
                throw new ArgumentNullException("inputBuffer");
            }
            if (inputOffset < 0)
            {
                throw new ArgumentOutOfRangeException("inputOffset");
            }
            if (inputCount < 0 || inputCount > inputBuffer.Length)
            {
                throw new ArgumentException("inputCount");
            }
            if (inputBuffer.Length - inputCount < inputOffset)
            {
                throw new ArgumentOutOfRangeException("inputOffset");
            }
            if (this.State == 0)
            {
                this.Initialize();
            }
            this.HashCore(inputBuffer, inputOffset, inputCount);
            this.HashValue = this.HashFinal();
            byte[] array = new byte[inputCount];
            System.Buffer.BlockCopy(inputBuffer, inputOffset, array, 0, inputCount);
            this.State = 0;
            return array;
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposing)
            {
                this.Initialize();
            }
        }

        public void Dispose()
        {
            this.Dispose(true);
        }

        static MD5()
        {
            // 注意: 此类型已标记为 'beforefieldinit'.
            byte[] array = new byte[64];
            array[0] = 128;
            MD5.PADDING = array;
        }
    }


    namespace Tools
    {

        public class return_dhid
        {
            public bool has { get; set; }
            public string dhid { get; set; }
            public string RawString { get; set; }
        }

        public class pwd
        {
            public string SSID { get; set; }
            public string BSSID { get; set; }
            public string Password { get; set; }
        }

        public class return_pwd : List<pwd>
        {
            public bool Success { get; set; }
            public string RawString { get; set; }
        }

        public enum WiFiStatu
        {
            //
            // 摘要:
            //     已连接至此WiFi
            Linked = 0,
            //
            // 摘要:
            //     可以连接至此WiFi
            HasPassword = 1,
            //
            // 摘要:
            //     此WiFi无密码
            Open = 2,
            //
            // 摘要:
            //     需要输入密码
            Normal = 3
        }

        public class Time
        {
            public static int UnixTime()
            {
                return (int)DateTime.Now.ToUniversalTime().Subtract(new DateTime(1970, 1, 1)).TotalSeconds;
            }
        }

        public class GZip
        {
            public static byte[] CompressionData(byte[] input)
            {
                byte[] temp = null;
                try
                {
                    using (MemoryStream ms = new MemoryStream())
                    {
                        using (GZipStream compressStream = new GZipStream(ms, CompressionMode.Compress, true))
                        {
                            //写入目标流
                            compressStream.Write(input, 0, input.Length);

                        }
                        temp = ms.ToArray();

                    }
                }
                catch (Exception ex)
                {
                    throw ex;
                }

                return temp;
            }

            public static byte[] DecompressionData(byte[] input)
            {
                byte[] temp = null;
                try
                {
                    using (MemoryStream baseData = new MemoryStream())
                    {

                        MemoryStream rmstemp = new MemoryStream(input);
                        using (GZipStream DecompressString = new GZipStream(rmstemp, CompressionMode.Decompress))
                        {
                            byte[] buff = new byte[4096];
                            int n;
                            while ((n = DecompressString.Read(buff, 0, buff.Length)) != 0)
                            {
                                baseData.Write(buff, 0, n);
                            }
                        }
                        rmstemp.Dispose();

                        temp = baseData.ToArray();
                    }
                }
                catch (Exception ex)
                {
                    temp = null;
                }
                return temp;
            }
        }

        public class AesHelper
        {

            public static byte[] Encrypt(byte[] b, string password, string iv)
            {
                Aes rijndaelCipher = Aes.Create();

                rijndaelCipher.Mode = CipherMode.CBC;

                rijndaelCipher.Padding = PaddingMode.None;

                rijndaelCipher.KeySize = 128;

                rijndaelCipher.BlockSize = 128;

                byte[] pwdBytes = System.Text.Encoding.UTF8.GetBytes(password);

                byte[] keyBytes = new byte[16];

                int len = pwdBytes.Length;

                if (len > keyBytes.Length) len = keyBytes.Length;

                System.Array.Copy(pwdBytes, keyBytes, len);

                rijndaelCipher.Key = keyBytes;


                byte[] ivBytes = System.Text.Encoding.UTF8.GetBytes(iv);
                rijndaelCipher.IV = ivBytes;

                ICryptoTransform transform = rijndaelCipher.CreateEncryptor();

                byte[] plainText = b;

                byte[] cipherBytes = transform.TransformFinalBlock(plainText, 0, plainText.Length);

                return cipherBytes;

            }
            public static byte[] Decrypt(byte[] Data, string Key, string Vector)
            {
                //Byte[] bKey = new Byte[32];
                //Array.Copy(Encoding.UTF8.GetBytes(Key.PadRight(bKey.Length)), bKey, bKey.Length);
                Byte[] bKey = Encoding.UTF8.GetBytes(Key);
                //Byte[] bVector = new Byte[16];
                //Array.Copy(Encoding.UTF8.GetBytes(Vector.PadRight(bVector.Length)), bVector, bVector.Length);
                Byte[] bVector = Encoding.UTF8.GetBytes(Vector);

                Byte[] original = null; // 解密后的明文 


                Aes Aes = Aes.Create();
                Aes.Mode = CipherMode.CBC;
                Aes.Padding = PaddingMode.None;
                try
                {
                    // 开辟一块内存流，存储密文 
                    using (MemoryStream Memory = new MemoryStream(Data))
                    {
                        // 把内存流对象包装成加密流对象 
                        using (CryptoStream Decryptor = new CryptoStream(Memory,
                        Aes.CreateDecryptor(bKey, bVector),
                        CryptoStreamMode.Read))
                        {
                            // 明文存储区 
                            using (MemoryStream originalMemory = new MemoryStream())
                            {
                                Byte[] Buffer = new Byte[1024];
                                Int32 readBytes = 0;
                                while ((readBytes = Decryptor.Read(Buffer, 0, Buffer.Length)) > 0)
                                {
                                    originalMemory.Write(Buffer, 0, readBytes);
                                }


                                original = originalMemory.ToArray();
                            }
                        }
                    }
                }
                catch (Exception e)
                {
                    original = null;
                }


                return original;
            }
        }

        public class StringHelper
        {
            public static byte[] stringToByteArray(string hexString)
            {
                hexString = hexString.Replace(" ", "");
                if ((hexString.Length % 2) != 0)
                    hexString += " ";
                byte[] returnBytes = new byte[hexString.Length / 2];
                for (int i = 0; i < returnBytes.Length; i++)
                    returnBytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
                return returnBytes;
            }

            public static string byteToHexStr(byte[] bytes)
            {
                string returnStr = "";
                if (bytes != null)
                {
                    for (int i = 0; i < bytes.Length; i++)
                    {
                        returnStr += bytes[i].ToString("X2");
                    }
                }
                return returnStr;
            }
        }
    }

}


