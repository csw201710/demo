package main

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"os"
)

func GetAllFiles(dirPth string) (files []string, err error) {
	var dirs []string
	dir, err := ioutil.ReadDir(dirPth)
	if err != nil {
		return nil, err
	}
	PthSep := string(os.PathSeparator)

	for _, fi := range dir {
		if fi.IsDir() {
			dirs = append(dirs, dirPth+PthSep+fi.Name())
			//GetAllFiles(dirPth + PthSep + fi.Name())
		} else { // 过滤指定格式
			//ok := strings.HasSuffix(fi.Name(), ".go")
			//if ok {
			//	files = append(files, dirPth+PthSep+fi.Name())
			//}
			files = append(files, dirPth+PthSep+fi.Name())
		}
	} // 读取子目录下文件
	for _, table := range dirs {
		temp, _ := GetAllFiles(table)
		for _, temp1 := range temp {
			files = append(files, temp1)
		}
	}
	return files, nil
}

func removeLineBreak(s *string) {
	size := len(*s)
	if size > 0 {

		if (*s)[size-1] == '\n' {
			drop := 1
			if size > 1 && (*s)[size-2] == '\r' {
				drop = 2
			}
			(*s) = (*s)[:size-drop]
		}
	}
}

func readFileLine(filePath string, startLine int, lineCount int) (lines []string) {
	f, err := os.Open(filePath)
	if err != nil {
		panic(err)
	}
	defer f.Close()
	rd := bufio.NewReader(f)

	startLine -= 1

	for i := 0; i < startLine; i++ {
		_, err := rd.ReadString('\n')
		if err != nil {
			if err == io.EOF {
				return
			}
			panic(err)
		}
	}
	for i := 0; i < lineCount; i++ {

		l, err := rd.ReadString('\n')
		if err != nil && err != io.EOF {
			panic(err)
		}

		if len(l) > 0 {
			removeLineBreak(&l)
			lines = append(lines, l)
		}

	}
	return
}

/*
func CompareInsensitive(a, b string) bool {
	// loop over string a and convert every rune to lowercase
	for i := 0; i < len(a); i++ {
		a[i] = unicode.ToLower(a[i])
	}
	// loop over string b and convert every rune to lowercase
	for i := 0; i < len(b); i++ {
		b[i] = unicode.ToLower(b[i])
	}
	// loop over both a and b and return false if there is a mismatch
	for i := 0; i < len(a); i++ {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}
*/

// 0 s1 == s2
// 1 s1 > s2
// 2 s1 < s2

func CompareStr(s1, s2 string) int {
	// loop over both a and b and return false if there is a mismatch
	var s_len int
	s1_len := len(s1)
	s2_len := len(s2)
	if s1_len >= s2_len {
		s_len = s2_len
	} else {
		s_len = s1_len
	}

	for i := 0; i < s_len; i++ {
		if s1[i] > s2[i] {
			return 1
		} else if s1[i] < s2[i] {
			return -1
		}
	}

	if s1_len > s2_len {
		return 1
	} else if s1_len < s2_len {
		return -1
	}
	return 0

}

// 0 已存在
// 非0 行数
func checkFile(filePath string, data string) (startLine int) {
	var flag1 int
	f, err := os.Open(filePath)
	if err != nil {
		panic(err)
	}
	defer f.Close()
	rd := bufio.NewReader(f)
	for i := 0; ; i++ {
		l, err := rd.ReadString('\n')
		if err != nil {
			if err == io.EOF {
				if i == 0 {
					startLine = 1
				} else {
					startLine = i + 1
				}
				break
			}
			panic(err)
		}
		if len(l) > 0 {
			removeLineBreak(&l)

			flag1 = CompareStr(l, data)
			if flag1 > 0 {
				startLine = i + 1
				break
			} else if flag1 == 0 {
				startLine = 0
				break
			}

		}
	}

	return
}

func insertData(filePath string, data string) (err error) {
	startPos := checkFile(filePath, data)
	if startPos% 100 == 0{
		fmt.Printf("insertData startPos : %d\n", startPos)	
	}
	
	if startPos != 0 {
		f, err := os.Open(filePath)
		if err != nil {
			panic(err)
		}

		tmpfile, err := ioutil.TempFile("./", "tmpfile")
		if err != nil {
			panic(err)
		}
		defer func() {
			f.Close()
			tmpfile.Close()

			err = os.Rename(tmpfile.Name(), filePath)
			if err != nil {
				panic(err)
			}
		}()

		rd := bufio.NewReader(f)

		data += "\r\n"

		var isWriteData int
		isWriteData = 0
		for i := 1; ; i++ {
			l, err := rd.ReadString('\n')
			if err != nil {
				if err == io.EOF {
					if i == 0 || isWriteData == 0 {
						tmpfile.WriteString(data)
					}

					break
				}
				panic(err)
			}
			if i == startPos {
				tmpfile.WriteString(data)
				isWriteData = 1
				tmpfile.WriteString(l)
			} else {
				tmpfile.WriteString(l)
			}
		}
	}
	return nil

}

func doing(filePath string) {
	f, err := os.Open(filePath)
	if err != nil {
		panic(err)
	}
	defer f.Close()
	rd := bufio.NewReader(f)

	for {
		l, err := rd.ReadString('\n')
		if err != nil {
			if err == io.EOF {
				return
			}
			panic(err)
		}
		if len(l) > 0 {
			removeLineBreak(&l)
			insertData(`D:\systemp\t000\2.txt`, l)
		}
	}
}

func main() {
	xfiles, _ := GetAllFiles(`D:\xxx`)
	for _, file := range xfiles {
		fmt.Printf("获取的文件为[%s]\n", file)
		doing(file)
	}
	/*
	lines := readFileLine(`D:\xxxxx\1.txt`, 2, 5)
	for _, l := range lines {
		fmt.Printf("line[%s]\n", l)
	}
	*/

}
