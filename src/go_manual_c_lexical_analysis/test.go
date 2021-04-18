package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

func IntToBytes(n int) []byte {
	x := int32(n)
	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}


func main() {
	//var abc interface{} = word.BlankWord{}
	//switch abc.(type) {
	//	case word.BlankWord:
	//		fmt.Println("aasdfa")
	//
	//}



	//var a int = 1000
	//var temp byte = byte(a)
	//fmt.Println(temp)


	//var str  = byte{0,1,2,3}


	var abc string = "\n"
	fmt.Println([]byte(abc))
}
