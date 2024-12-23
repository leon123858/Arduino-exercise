# 定義變數
$PORT = "COM3"
$FQBN = "arduino:avr:uno"
$SketchPath = "sketch" # 假設 sketch 資料夾在當前目錄

# 定義函數 (相當於 Makefile 的 target)
function Compile {
  & "arduino-cli" compile --fqbn $FQBN $SketchPath
  if ($LASTEXITCODE -ne 0) {
    Write-Error "編譯失敗"
    exit 1
  }
}

function Upload {
  & "arduino-cli" upload -p $PORT --fqbn $FQBN $SketchPath
    if ($LASTEXITCODE -ne 0) {
    Write-Error "上傳失敗"
    exit 1
  }
}

# 主程式邏輯 (相當於 Makefile 的 all target)
function Main {
  Compile
  Upload
}

# 執行主程式
Main