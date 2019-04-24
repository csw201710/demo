package my.debug;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.zip.GZIPInputStream;
import java.util.zip.ZipInputStream;

import org.apache.commons.io.FileUtils;
import org.bouncycastle.apache.bzip2.CBZip2InputStream;

import com.jcraft.jzlib.ZInputStream;

public class Test {
	public static void main(String[] args) throws IOException {

		byte[] m = FileUtils.readFileToByteArray(new java.io.File("d:\\CET_TRAINER.CETRAINER"));
		for (int i = 2; i <= m.length - 1; i++) {
			m[i] = (byte) (m[i] ^ m[i - 2]) ;
		}
		for (int i = m.length - 2; i >= 0; i--) {
			m[i] = (byte) (m[i] ^ m[i + 1]);
		}
		byte k = (byte) 0xce;
		for (int i = 0; i <= m.length - 1; i++) {
			m[i] = (byte) (m[i] ^ k);
			k = (byte) (k+1);
		}
		byte[] p = new byte[5];
		System.arraycopy(m, 0, p, 0, 5);
		//p[5] = 0x00;
		if (Arrays.equals(p, "CHEAT".getBytes())) {
			byte [] m1 = new byte[m.length - 5];
			byte [] m2 = new byte[m1.length - 4];
			
			System.arraycopy(m, 5, m1, 0, m.length - 5);
			int size = byteArrayToInt(m1);
			System.arraycopy(m1, 4, m2, 0, m1.length - 4);
			byte [] m3 = unZip(m2);
			FileUtils.writeByteArrayToFile(new File("d:\\out.txt"), m3);
		}else {
			byte [] m1 = unZip(m);
			FileUtils.writeByteArrayToFile(new File("d:\\out.txt"), m1);
		}

	}
	/***
	  * 解压BZip2
	  * 
	  * @param data
	  * @return
	  */
	 public static byte[] unBZip2(byte[] data) {
	  byte[] b = null;
	  try {
	   ByteArrayInputStream bis = new ByteArrayInputStream(data);
	   CBZip2InputStream bzip2 = new CBZip2InputStream(bis);
	   byte[] buf = new byte[1024];
	   int num = -1;
	   ByteArrayOutputStream baos = new ByteArrayOutputStream();
	   while ((num = bzip2.read(buf, 0, buf.length)) != -1) {
	    baos.write(buf, 0, num);
	   }
	   b = baos.toByteArray();
	   baos.flush();
	   baos.close();
	   bzip2.close();
	   bis.close();
	  } catch (Exception ex) {
	   ex.printStackTrace();
	  }
	  return b;
	 }
	/**
	  *jzLib压缩的数据
	  * 
	  * @param object
	  * @return
	  * @throws IOException
	  */
	 public static byte[] unjzlib(byte[] object) {
	  byte[] data = null;
	  try {
	   ByteArrayInputStream in = new ByteArrayInputStream(object);
	   ZInputStream zIn = new ZInputStream(in);
	   byte[] buf = new byte[1024];
	   int num = -1;
	   ByteArrayOutputStream baos = new ByteArrayOutputStream();
	   while ((num = zIn.read(buf, 0, buf.length)) != -1) {
	    baos.write(buf, 0, num);
	   }
	   data = baos.toByteArray();
	   baos.flush();
	   baos.close();
	   zIn.close();
	   in.close();
	   
	  } catch (IOException e) {
	   e.printStackTrace();
	  }
	  return data;
	 }

	public static int byteArrayToInt(byte[] b) {
		return b[3] & 0xFF | (b[2] & 0xFF) << 8 | (b[1] & 0xFF) << 16 | (b[0] & 0xFF) << 24;
	}

	public static byte[] intToByteArray(int a) {
		return new byte[] { (byte) ((a >> 24) & 0xFF), (byte) ((a >> 16) & 0xFF), (byte) ((a >> 8) & 0xFF),
				(byte) (a & 0xFF) };
	}

	/***
	  * 解压GZip
	  * 
	  * @param data
	  * @return
	  */
	 public static byte[] unGZip(byte[] data) {
	  byte[] b = null;
	  try {
	   ByteArrayInputStream bis = new ByteArrayInputStream(data);
	   GZIPInputStream gzip = new GZIPInputStream(bis);
	   byte[] buf = new byte[1024];
	   int num = -1;
	   ByteArrayOutputStream baos = new ByteArrayOutputStream();
	   while ((num = gzip.read(buf, 0, buf.length)) != -1) {
	    baos.write(buf, 0, num);
	   }
	   b = baos.toByteArray();
	   baos.flush();
	   baos.close();
	   gzip.close();
	   bis.close();
	  } catch (Exception ex) {
	   ex.printStackTrace();
	  }
	  return b;
	 }
	public static byte[] unZip(byte[] data) {
		byte[] b = null;
		try {
			ByteArrayInputStream bis = new ByteArrayInputStream(data);
			ZipInputStream zip = new ZipInputStream(bis);
			while (zip.getNextEntry() != null) {
				byte[] buf = new byte[1024];
				int num = -1;
				ByteArrayOutputStream baos = new ByteArrayOutputStream();
				while ((num = zip.read(buf, 0, buf.length)) != -1) {
					baos.write(buf, 0, num);
				}
				b = baos.toByteArray();
				baos.flush();
				baos.close();
			}
			zip.close();
			bis.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return b;
	}
}


