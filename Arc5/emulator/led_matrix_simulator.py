import tkinter as tk
import numpy as np
from tkinter import ttk

class LEDMatrixSimulator:
    def __init__(self, root):
        self.root = root
        self.root.title("LED矩阵仿真器（TC4标定用）")
        self.root.geometry("800x400")
        
        # 1. 核心参数定义
        self.led_count = 8  # 8个LED
        self.is_running = False  # 开关状态
        self.current_vector = np.array([[1, 0, 0, 0, 0, 0, 0, 0]])  # 初始向量（第1个LED亮）
        # 默认循环移位矩阵（可自定义修改）
        self.shift_matrix = np.array([
            [0, 1, 0, 0, 0, 0, 0, 0],
            [0, 0, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 1, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 0, 1, 0, 0],
            [0, 0, 0, 0, 0, 0, 1, 0],
            [0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0]
        ])
        
        # 2. 创建GUI组件
        self._create_gui()
        
        # 3. 启动灯效循环
        self._update_leds()

    def _create_gui(self):
        # LED显示区域
        self.led_frames = []
        led_frame = ttk.LabelFrame(self.root, text="8路LED灯效预览")
        led_frame.pack(pady=20, padx=20, fill=tk.X)
        
        for i in range(self.led_count):
            # 每个LED用Label模拟，初始灰色（熄灭）
            led = tk.Label(
                led_frame, 
                text=f"LED{i+1}", 
                bg="gray", 
                width=8, 
                height=4, 
                font=("Arial", 12, "bold")
            )
            led.pack(side=tk.LEFT, padx=5, pady=10)
            self.led_frames.append(led)
        
        # 控制开关
        self.switch_btn = ttk.Button(
            self.root,
            text="打开灯效（当前：关闭）",
            command=self._toggle_switch
        )
        self.switch_btn.pack(pady=10)
        
        # 矩阵参数显示（方便标定后复制到TC4）
        matrix_frame = ttk.LabelFrame(self.root, text="当前变化矩阵（可修改代码中shift_matrix）")
        matrix_frame.pack(pady=10, padx=20, fill=tk.X)
        
        matrix_text = tk.Text(matrix_frame, height=8, width=50)
        matrix_text.insert(tk.END, np.array2string(self.shift_matrix, separator=", "))
        matrix_text.config(state=tk.DISABLED)  # 只读，防止误改
        matrix_text.pack(padx=10, pady=10)

    def _toggle_switch(self):
        # 切换开关状态
        self.is_running = not self.is_running
        if self.is_running:
            self.switch_btn.config(text="关闭灯效（当前：打开）")
        else:
            self.switch_btn.config(text="打开灯效（当前：关闭）")
            # 关闭时所有LED熄灭
            for led in self.led_frames:
                led.config(bg="gray")

    def _update_leds(self):
        # 核心：向量 × 矩阵 计算新状态
        if self.is_running:
            # 线性代数运算：行向量 × 矩阵（numpy自动处理点积）
            self.current_vector = np.dot(self.current_vector, self.shift_matrix)
            # 取整（避免浮点误差），0=灭，非0=亮
            led_states = (self.current_vector[0] > 0).astype(int)
            
            # 更新LED显示：亮=绿色，灭=灰色
            for i, state in enumerate(led_states):
                self.led_frames[i].config(bg="green" if state == 1 else "gray")
        
        # 定时刷新（200ms一次，可调整速度）
        self.root.after(200, self._update_leds)

if __name__ == "__main__":
    # 确保numpy和tkinter可用（tkinter是Python内置，numpy需安装：pip install numpy）
    try:
        root = tk.Tk()
        app = LEDMatrixSimulator(root)
        root.mainloop()
    except ImportError:
        print("请先安装numpy：执行命令 pip install numpy")
    except Exception as e:
        print(f"运行出错：{e}")