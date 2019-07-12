package main

import (
	"fmt"
	"github.com/axgle/mahonia"
)

func ConvertToString(src string, srcCode string, tagCode string) string {
	srcCoder := mahonia.NewDecoder(srcCode)
	srcResult := srcCoder.ConvertString(src)
	tagCoder := mahonia.NewDecoder(tagCode)
	_, cdata, _ := tagCoder.Translate([]byte(srcResult), true)
	result := string(cdata)
	return result
}

func GbkToUtf8(src string) string {
	return ConvertToString(src, "gbk", "utf-8")
}
func Utf8ToGbk(src string) string {
	return ConvertToString(src, "utf-8", "gbk")
}

func testConvert() {
	fmt.Println(Utf8ToGbk("小小"))
	fmt.Println(GbkToUtf8("小小"))
	fmt.Println("小小")
}
