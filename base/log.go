package base

const (
	Ok = iota
	Err
	Warn
)

func LpmLog(msg string, logLevel int) {

	switch logLevel {
	case Ok:
		println("lpm:", msg)
		return
	case Err:
		println("lpm err:", msg)
		return
	case Warn:
		println("lpm warn:", msg)
		return
	}
}
