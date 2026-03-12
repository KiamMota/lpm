class Lpm < Formula
  desc "Terminal-first CLI companion for lazy.nvim"
  homepage "https://github.com/KiamMota/lpm"
  version "1.0.0"

  on_macos do
    if Hardware::CPU.arm?
      url "https://github.com/KiamMota/lpm/releases/download/v#{version}/lpm_1.0.0_darwin_arm64.tar.gz"
      sha256 "52f83f22b30bf36f6e4a14f67a60bca3c481cb70ae59bc16e4fe3fb5b846f80c"
    else
      url "https://github.com/KiamMota/lpm/releases/download/v#{version}/lpm_1.0.0_darwin_amd64.tar.gz"
      sha256 "948004af0b6e1949f988b8958ad4970635189a5e8a9236aee579e03a37bf84eb"
    end
  end

  on_linux do
    if Hardware::CPU.arm?
      url "https://github.com/KiamMota/lpm/releases/download/v#{version}/lpm_1.0.0_linux_arm64.tar.gz"
      sha256 "b9ae71e5dd750c3bed348992b32a0402273342013eaa5e762f08c7f501cc5a1d"
    else
      url "https://github.com/KiamMota/lpm/releases/download/v#{version}/lpm_1.0.0_linux_amd64.tar.gz"
      sha256 "f3361f27a4c33c7dbe3c13de23dac7f0f73ad0ca7d98c2246a036c9cc1d1d299"
    end
  end

  def install
    bin.install "lpm"
  end

  test do
    system "#{bin}/lpm", "--version"
  end
end
