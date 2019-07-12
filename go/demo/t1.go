package main

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"sort"
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
		if err != nil {
			if err == io.EOF {
				break
			}
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
	if startPos%100 == 0 {
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

func readFileAllLine(filePath string, lines []string) []string {
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
				break
			}

			panic(err)
		}

		if len(l) > 0 {
			removeLineBreak(&l)
			lines = append(lines, l)
		}

	}
	return lines
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
			insertData(`D:\xxxx\x.txt`, l)
		}
	}
}

func InsertSort(values []string) {
	length := len(values)
	if length <= 1 {
		return
	}

	for i := 1; i < length; i++ {
		tmp := values[i] // 从第二个数开始，从左向右依次取数
		key := i - 1     // 下标从0开始，依次从左向右

		// 每次取到的数都跟左侧的数做比较，如果左侧的数比取的数大，就将左侧的数右移一位，直至左侧没有数字比取的数大为止
		for key >= 0 && (CompareStr(tmp, values[key]) < 0) {
			values[key+1] = values[key]
			key--
			//fmt.Println(values)
		}

		// 将取到的数插入到不小于左侧数的位置
		if key+1 != i {
			values[key+1] = tmp
		}
		//fmt.Println(values)
	}
}

func Rm_Duplicate(list []string) []string {
	var x []string = []string{}
	for _, i := range list {
		if len(x) == 0 {
			x = append(x, i)
		} else {
			for k, v := range x {
				if i == v {
					break
				}
				if k == len(x)-1 {
					x = append(x, i)
				}
			}
		}
	}
	return x
}


type SubList []string
func (p SubList) Len() int{
	 return len(p) 
}
func (p SubList) Swap(i, j int){
	 p[i], p[j] = p[j], p[i] 
}

func (p SubList) Less(i, j int) bool {
	 return CompareStr(p[i] , p[j]) > 0 
}

func main() {
	xfiles, _ := GetAllFiles(`D:\xxx`)
	var lines SubList

	for _, file := range xfiles {
		fmt.Printf("获取的文件为[%s]\n", file)
		//doing(file)
		
		lines = readFileAllLine(file, lines)
		fmt.Printf("InsertSort before len[%d]\n", len(lines))
		//InsertSort(lines)
		//lines = Rm_Duplicate(lines)
		fmt.Printf("Sort before len[%d]\n", len(lines))
		if false{
			//sort.Sort(lines)
			sort.Stable(lines)
		}
		fmt.Printf("lines len[%d]\n", len(lines))
	}

	/*
		lines := readFileLine(`D:\xxxx.txt`, 2, 5)
		for _, l := range lines {
			fmt.Printf("line[%s]\n", l)
		}
	*/

}


