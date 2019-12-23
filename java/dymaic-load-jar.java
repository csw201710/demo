package my.lt;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

public class MxLauncher {
	public static final boolean DEBUG = false;
	private static final Class<?>[] PARAMETERS = new Class[] { URL.class };

	public static void main(String[] args) throws Exception {
		String appClassName = "";
		String appPaths = "lib, ext";
		Enumeration<URL> resources = MxLauncher.class.getClassLoader().getResources("META-INF/MANIFEST.MF");

		while (resources.hasMoreElements()) {
			try {
				Manifest manifest = new Manifest(((URL) resources.nextElement()).openStream());
				Attributes attributes = manifest.getMainAttributes();
				String className = attributes.getValue("mxMain-Class");
				if (className != null && className.trim().length() > 0) {
					appClassName = className;
				}
				String paths = attributes.getValue("mxMain-Paths");
				if (paths != null && paths.trim().length() > 0) {
					appPaths = paths;
				}
			} catch (IOException E) {
			}
		}

		if (appClassName == null || appClassName.trim().length() == 0) {

			System.err.println("Please define mxMain-Class in your launcher jar manifest!");
			System.exit(-1);
		}

		String[] folders = appPaths.split(",");
		List<File> jars = new ArrayList<File>();
		for (String folder : folders) {
			if (folder != null) {

				File libFolder = new File(folder.trim());
				if (libFolder.exists()) {

					List<File> found = findJars(libFolder.getAbsoluteFile());
					jars.addAll(found);
				}
			}
		}
		Collections.sort(jars);
		Collections.reverse(jars);

		if (jars.size() == 0) {
			for (String folder : folders) {
				File libFolder = new File(folder);
				if (libFolder.exists()) {
					System.err.println("Failed to find any JARs in " + libFolder.getPath());
				}
			}
		} else {
			for (File jar : jars) {
				try {
					jar.canRead();
					addJarFile(jar);
				} catch (Throwable t) {
					t.printStackTrace();
				}
			}
		}

		Class<?> appClass = Class.forName(appClassName);
		Method main = appClass.getMethod("main", new Class[] { String[].class });
		main.invoke(null, new Object[] { args });
	}

	public static List<File> findJars(File folder) {
		List<File> jars = new ArrayList<File>();
		if (folder.exists()) {
			File[] libs = folder.listFiles(new FileFilter() {
				public boolean accept(File file) {
					return (!file.isDirectory() && file.getName().toLowerCase().endsWith(".jar"));
				}
			});
			if (libs != null && libs.length > 0) {
				jars.addAll(Arrays.asList(libs));
			}
		}

		return jars;
	}

	public static void addJarFile(File f) throws IOException {
		if (f.getName().indexOf("-sources") > -1 || f.getName().indexOf("-javadoc") > -1) {
			return;
		}

		URL u = f.toURI().toURL();

		URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
		Class<?> sysclass = URLClassLoader.class;
		try {
			Method method = sysclass.getDeclaredMethod("addURL", PARAMETERS);
			method.setAccessible(true);
			method.invoke(sysloader, new Object[] { u });
		} catch (Throwable t) {
			throw new IOException(MessageFormat.format("Error, could not add {0} to system classloader",
					new Object[] { f.getPath() }), t);
		}
	}
}



