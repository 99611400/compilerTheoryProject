package word

//
//type Double struct {
//	Word
//	//Separator separator;
//	//BlankWord blankWord;
//	//Operator operator1;
//
//}
//
//
//
//
//
//func (d *Double) Parse(str []byte,pointer int) bool{
//	if !d.IsBegin(str[pointer]){
//		return false
//	}
//	var tempStr = str[pointer:]
//
//	for  index,c := range tempStr{
//		if d.IsEnd(c){
//			d.wordString[index] = 0
//			return true
//		}
//		if tempStr[index] >= '0' && tempStr[index] <= '9'{
//			d.wordString[index] = tempStr[index]
//		} else {
//			return false
//		}
//	}
//	return true
//}
//
//func (d *Double) format() []byte{
//	if d.Word.wordString[0] == '\n' {
//		return []byte("\\n")
//	}else {
//		return d.Word.wordString
//	}
//}
//
//func (d *Double) IsBegin(c byte) bool{
//	if c == '+'{
//		return true
//	}
//	if c == '-'{
//		return true
//	}
//	return c >= '0' && c <= '9'
//}
//
//func (d *Double) IsEnd(c byte) bool{
//
//}
//
//func (d *Double) WordPrint() {
//	fmt.Println("Double print")
//}
//func (d *Double) GetTypeCode() int{
//	return 3
//}
//
//
//func (d *Double) GetWordContent() []byte{
//	return d.wordString[0:getWordLength(d.wordString)]
//}