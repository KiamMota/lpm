package base

const (
	ColorRed    = "\033[0;31m"
	ColorOrange = "\033[38;2;255;165;0m"
	ColorGreen  = "\033[0;32m"
	ColorReset  = "\033[0m"
)

func Log(msg string, logLevel string) {

	switch logLevel {
	case ColorOrange:
		println(ColorOrange)
	case ColorGreen:
		println(ColorGreen)
	case ColorRed:
		println(ColorRed)
	default:
		break
	}
	println("lpm >>", msg)
	println(ColorReset)
}
