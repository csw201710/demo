package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"time"
	"unsafe"
)

func generateNum(filePath string, size int) (totalSize int64) {
	var s string
	var f *os.File
	var err error

	if size == 0 {
		return
	}
	for i := 0; i < size; i++ {
		s += "9"
	}
	size, _ = strconv.Atoi(s)
	fmt.Printf("%d\n", size)

	if len(filePath) != 0 {
		f, err = os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
		if err != nil {
			fmt.Println(err)
			return
		}
	}
	var size_len int = len(strconv.Itoa(size))
	if f != nil {
		writer := bufio.NewWriterSize(f, 40960)
		defer func() {
			writer.Flush()
			f.Close()
		}()
		for i := 0; i <= size; i++ {
			s = fmt.Sprintf("%0*d\n", size_len, i)
			writer.WriteString(s)
			totalSize += int64(len(s))
			if i%10000000 == 0 {
				fmt.Print(s)
			}
		}
	} else {
		for i := 0; i <= size; i++ {
			s = fmt.Sprintf("%0*d\n", size_len, i)
			totalSize += int64(len(s))
		}
	}

	return
}

func generateAlpha(filePath string, alphaSize int) (totalSize int64) {
	var s string
	var f *os.File
	var err error
	if alphaSize == 0 {
		return
	}

	var charArr []byte = make([]byte, alphaSize)
	for i := 0; i < alphaSize; i++ {
		charArr[i] = 'a'
	}

	if len(filePath) != 0 {
		f, err = os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
		if err != nil {
			fmt.Println(err)
			return
		}
	}
	if f != nil {
		writer := bufio.NewWriterSize(f, 40960)
		defer func() {
			writer.Flush()
			f.Close()
		}()
		for {

			s = fmt.Sprintf("%s\n", *(*string)(unsafe.Pointer(&charArr)))
			writer.WriteString(s)
			totalSize += int64(len(s))
			//fmt.Print(s)
			ret := getNextAlpha(charArr)
			if ret == 1 {
				break
			}
		}
	} else {
		for {
			s = fmt.Sprintf("%s\n", *(*string)(unsafe.Pointer(&charArr)))
			totalSize += int64(len(s))
			fmt.Print(s)
			ret := getNextAlpha(charArr)
			if ret == 1 {
				break
			}
		}
	}

	return
}

func generateAlphaAndNum(filePath string, alphaSize int, numSize int) (totalSize int64) {
	var s string
	var f *os.File
	var err error

	if alphaSize == 0 || numSize == 0 {
		return
	}

	var charArr []byte = make([]byte, alphaSize)
	for i := 0; i < alphaSize; i++ {
		charArr[i] = 'a'
	}

	for i := 0; i < numSize; i++ {
		s += "9"
	}

	numSize, _ = strconv.Atoi(s)
	fmt.Printf("totoal num : %d\n", numSize)

	if len(filePath) != 0 {
		f, err = os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
		if err != nil {
			fmt.Println(err)
			return
		}
	}
	var size_len int = len(strconv.Itoa(numSize))
	if f != nil {
		writer := bufio.NewWriterSize(f, 40960)
		defer func() {
			writer.Flush()
			f.Close()
		}()

		for {
			for i := 0; i <= numSize; i++ {
				s = fmt.Sprintf("%s%0*d\n", *(*string)(unsafe.Pointer(&charArr)), size_len, i)
				writer.WriteString(s)
				totalSize += int64(len(s))
				if i%10000000 == 0 {
					fmt.Print(s)
				}
			}

			if alphaSize == 0 {
				break
			}

			ret := getNextAlpha(charArr)
			if ret == 1 {
				break
			}

		}

	} else {
		for {
			for i := 0; i <= numSize; i++ {
				s = fmt.Sprintf("%s%0*d\n", *(*string)(unsafe.Pointer(&charArr)), size_len, i)
				totalSize += int64(len(s))
				fmt.Print(s)
			}

			if alphaSize == 0 {
				break
			}

			ret := getNextAlpha(charArr)
			if ret == 1 {
				break
			}
		}
	}

	return
}

func getNextAlpha(s []byte) int {
	s_len := len(s)
	if s_len > 0 {
		for j := s_len - 1; j >= 0; j-- {
			if s[j] == 'z' || s[j] == 'Z' {
				continue
			}
			s[j] += 1
			for i := j + 1; i < s_len; i++ {
				if s[i] >= 'a' && s[i] <= 'z' {
					s[i] = 'a'
				} else if s[i] >= 'A' && s[i] <= 'Z' {
					s[i] = 'A'
				}
			}
			return 0
		}
	}
	return 1
}

func formatSize(size int64) {
	var kb int64 = 1024 * 1
	var mb int64 = 1024 * 1024 * 1
	var gb int64 = 1024 * 1024 * 1024 * 1

	fmt.Printf("%v B total, %v G %v M %v KB %v B\n", size, size/gb, (size%gb)/mb, (size%(mb))/kb, size%kb)
}

func main() {
	t1 := time.Now()
	//size := generateNum(``, 11)
	var num int = 1
	var alpha int = 2
	var filePath string
	if false {
		filePath = fmt.Sprintf("E:\\dict\\%da-%dnum.txt", alpha, num)
	}

	//size := generateAlphaAndNum(filePath, alpha, num)
	size := generateAlpha(filePath, 5)

	formatSize(size)

	elapsed := time.Since(t1)
	fmt.Println("App elapsed: ", elapsed)
}


