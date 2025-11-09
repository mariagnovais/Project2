from typing import Optional, List, Tuple
import tkinter as tk
from tkinter import ttk, messagebox
import subprocess, sys
from pathlib import Path

def make_label(parent: tk.Widget, text: str, **grid_kwargs: int) -> tk.Label:
    label = tk.Label(parent, text=text, anchor="w", font=("Courier New", 16), fg = "#1d2c79", bg="white")
    label.grid(sticky="ew", pady=(8, 2), **grid_kwargs)
    return label

# ALL STATES
STATE_OPTIONS = (
    "Any", "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA",
    "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA",
    "RI", "SC", "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY"
)

CONTROL_OPTIONS = ("Any", "Public", "Private")
SIZE_OPTIONS = ("Any", "Small", "Medium", "Large")
TUITION_OPTIONS = ("Any", "<$20k", "$20k-$40k", "$40k-$60k", ">$60k")
ACCEPT_OPTIONS = ("Any", "<20%", "20%-50%", "50%-80%", ">80%")

def parse_matches(output : str) -> List[Tuple[str, float]]:
    rows: List[Tuple[str, float]] = []
    for line in output.splitlines():
        line = line.strip()
        if not line:
            continue
        try:
            if "," in line:
                name, score = line.rsplit(",", 1)
                rows.append((name.strip(), float(score.strip())))
            else:
                rows.append((line, 0.0))
        except Exception:
            rows.append((line, 0.0))
    return rows

class WelcomeFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_start, on_exit) -> None:
        super().__init__(master, padding=32)
        self.configure(style="Card.TFrame")

        title = tk.Label(self, text="College Matcher", font=("Courier New", 36, "bold"), fg="#1936a3", bg="white")
        title.grid(row=0, column=0, pady=(0, 20))

        emoji = tk.Label(self, text="🎓", font=("Arial", 48), bg="white")
        emoji.grid(row=0, column=1, padx=(16, 0), sticky="w")

        start_button = tk.Button(
            self,
            text="Start Matching",
            command=on_start,
            bg="#1936a3",
            fg="white",
            font=("Courier New", 18, "bold"),
            activebackground="#0f1f5c",
            relief="flat",
            padx=40,
            pady=16,
        )
        start_button.grid(row=1, column=0, columnspan=2, pady=(40, 16))

        exit_button = tk.Button(
            self,
            text="Exit",
            command=on_exit,
            bg="#1936a3",
            fg="white",
            font=("Courier New", 18, "bold"),
            activebackground="#0f1f5c",
            relief="flat",
            padx=40,
            pady=16,
        )
        exit_button.grid(row=2, column=0, columnspan=2, pady=(0, 10))

        for column in range(2):
            self.columnconfigure(column, weight=1)

        start_button.focus_set()

class FormFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_back, on_next) -> None:
        super().__init__(master, padding=32)
        self.configure(style="Card.TFrame")
        self.on_back = on_back
        self.on_next = on_next

        title = tk.Label(self, text="Enter your preferences:", font=("Courier New", 28, "bold"), fg="#1d2c79", bg="white")
        title.grid(row=0, column=0, columnspan=2, sticky="w", pady=(0, 20))

        # Variables
        self.name_var = tk.StringVar()
        self.sat_var = tk.StringVar()
        self.state_var = tk.StringVar(value="Any")
        self.control_var = tk.StringVar(value="Any")
        self.size_var = tk.StringVar(value="Any")
        self.tuition_var = tk.StringVar(value="Any")
        self.acceptance_var = tk.StringVar(value="Any")

        # Name (Entry)
        make_label(self, "Name:", row=1, column=0)
        name_entry = ttk.Entry(self, textvariable=self.name_var, font=("Courier New", 16))
        name_entry.grid(row=2, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        # SAT (Entry)
        make_label(self, "SAT Score:", row=3, column=0)
        sat_entry = ttk.Entry(self, textvariable=self.sat_var, font=("Courier New", 16))
        sat_entry.grid(row=4, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        # State (Dropdown)
        make_label(self, "State:", row=5, column=0)
        state_combo = ttk.Combobox(
            self,
            textvariable=self.state_var,
            values=STATE_OPTIONS,
            font=("Courier New", 16),
            state="readonly",
        )
        state_combo.grid(row=6, column=0, columnspan=2, sticky="ew", pady=(0, 12))
        
        # Control (Dropdown)
        make_label(self, "Control:", row=7, column=0)
        control_combo = ttk.Combobox(
            self,
            textvariable=self.control_var,
            values=CONTROL_OPTIONS,
            font=("Courier New", 16),
            state="readonly",
        )
        control_combo.grid(row=8, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        # Size (Dropdown)
        make_label(self, "Campus Size:", row=9, column=0)
        size_combo = ttk.Combobox(
            self,
            textvariable=self.size_var,
            values=SIZE_OPTIONS,
            font=("Courier New", 16),
            state="readonly",
        )
        size_combo.grid(row=10, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        # Tuition (Dropdown)
        make_label(self, "Tuition:", row=11, column=0)
        tuition_combo = ttk.Combobox(
            self,
            textvariable=self.tuition_var,
            values=TUITION_OPTIONS,
            font=("Courier New", 16),
            state="readonly",
        )
        tuition_combo.grid(row=12, column=0, columnspan=2, sticky="ew", pady=(0, 12))

        # Acceptance Rate (Dropdown)
        make_label(self, "Acceptance Rate:", row=13, column=0)
        acceptance_combo = ttk.Combobox(
            self,
            textvariable=self.acceptance_var,
            values=ACCEPT_OPTIONS,
            font=("Courier New", 16),
            state="readonly",
        )
        acceptance_combo.grid(row=14, column=0, columnspan=2, sticky="ew", pady=(0, 16))

        # Buttons
        button_frame = ttk.Frame(self, style="Card.TFrame")
        button_frame.grid(row=15, column=0, columnspan=2, pady=(16, 0), sticky="e")

        ttk.Button(button_frame, text="Back", command=self.on_back, style="Accent.TButton").grid(row=0, column=0, padx=6)
        ttk.Button(button_frame, text="Next: Set Weights", command=self.submit_form, style="Accent.TButton").grid(row=0, column=1, padx=6)

        for column in range(2):
            self.columnconfigure(column, weight=1)

        name_entry.focus_set()
    
    def submit_form(self) -> None:
        # Validate SAT score
        sat_value = self.sat_var.get().strip()
        if sat_value and not sat_value.isdigit():
            messagebox.showerror("Invalid SAT", "Please enter a number (e.g., 1400)")
            return
        
        form_data = {
            "name": self.name_var.get().strip(),
            "sat": sat_value,
            "state": self.state_var.get().strip(),
            "control": self.control_var.get().strip(),
            "size": self.size_var.get().strip(),
            "tuition": self.tuition_var.get().strip(),
            "acceptance": self.acceptance_var.get().strip(),
        }

        self.on_next(form_data)


class WeightsFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_back, on_results, form_data: dict) -> None:
        super().__init__(master, padding=32)
        self.configure(style="Card.TFrame")
        self.on_back = on_back
        self.on_results = on_results
        self.form_data = form_data # Store data from previous screen

        title = tk.Label(self, text="Set Importance Weights", font=("Courier New", 28, "bold"), fg="#1d2c79", bg="white")
        title.grid(row=0, column=0, columnspan=2, sticky="w", pady=(0, 20))

        # Variables for weights
        self.tuition_weight_var = tk.DoubleVar(value=1.0)
        self.acceptance_weight_var = tk.DoubleVar(value=1.0)
        self.sat_weight_var = tk.DoubleVar(value=1.0)
        self.size_weight_var = tk.DoubleVar(value=1.0)

        # Helper function for creating a slider
        def create_slider(parent, text, variable, row):
            make_label(parent, text, row=row, column=0)
            # Display current value
            value_label = tk.Label(parent, textvariable=variable, font=("Courier New", 16, "bold"), fg="#1936a3", bg="white")
            value_label.grid(row=row, column=1, sticky="e", padx=(10,0))
            # Slider
            slider = ttk.Scale(
                parent,
                from_=1.0,
                to=5.0,
                orient="horizontal",
                variable=variable,
                command=lambda v: variable.set(round(float(v), 1)) # Round to 1 decimal
            )
            slider.grid(row=row+1, column=0, columnspan=2, sticky="ew", pady=(0, 20))

        # Create sliders
        create_slider(self, "Tuition Importance:", self.tuition_weight_var, 1)
        create_slider(self, "Acceptance Rate Importance:", self.acceptance_weight_var, 3)
        create_slider(self, "SAT Score Importance:", self.sat_weight_var, 5)
        create_slider(self, "Campus Size Importance:", self.size_weight_var, 7)
        
        # Add radio buttons for algorithm choice
        make_label(self, "Algorithm:", row=9, column=0)
        self.algo_var = tk.StringVar(value="hashmap")

        algo_frame = ttk.Frame(self, style="Card.TFrame")
        algo_frame.grid(row=10, column=0, columnspan=2, sticky="w", pady=(0, 20))

        ttk.Radiobutton(
            algo_frame,
            text = "Hashmap",
            variable=self.algo_var,
            value="hashmap",
            style="TRadiobutton"
        ).pack(side = "left", padx=20)

        ttk.Radiobutton(
            algo_frame,
            text = "Heap",
            variable=self.algo_var,
            value="heap",
            style="TRadiobutton"
        ).pack(side = "left", padx=20)

        # Buttons
        button_frame = ttk.Frame(self, style="Card.TFrame")
        button_frame.grid(row=11, column=0, columnspan=2, pady=(16, 0), sticky="e")

        ttk.Button(button_frame, text="Back", command=self.on_back, style="Accent.TButton").grid(row=0, column=0, padx=6)
        ttk.Button(button_frame, text="Run Match", command=self.run_matcher, style="Accent.TButton").grid(row=0, column=1, padx=6)

        self.columnconfigure(0, weight=1)

    def run_matcher(self) -> None:
        sat_value = self.form_data.get("sat", "")
        state_str = self.form_data.get("state", "Any")
        control_str = self.form_data.get("control", "Any")
        tuition_str = self.form_data.get("tuition", "Any")
        acceptance_str = self.form_data.get("acceptance", "Any")
        size_str = self.form_data.get("size", "Any")

        tuition_map = {
            "Any": 0,
            "<$20k": 20000,
            "$20k-$40k": 40000,
            "$40k-$60k": 60000,
            ">$60k": 150000,
        }

        acceptance_map = {
            "Any": 0.0,
            "<20%" : 0.01, 
            "20%-50%": 0.2, 
            "50%-80%": 0.5, 
            ">80%" : 0.8,
        }
        
        size_map = {
            "Any": 1, 
            "Small": 500, 
            "Medium": 5000, 
            "Large": 15000,
        }

        state = "" if state_str == "Any" else state_str
        control = "" if control_str == "Any" else control_str
        tuition = tuition_map.get(tuition_str, 0)
        acceptance_rate = acceptance_map.get(acceptance_str, 0)
        school_size = size_map.get(size_str, 0)
        sat_score = sat_value

        weight_tuition = str(self.tuition_weight_var.get())
        weight_acceptance = str(self.acceptance_weight_var.get())
        weight_sat = str(self.sat_weight_var.get())
        weight_size = str(self.size_weight_var.get())

        algorithm_choice = self.algo_var.get()

        project_root = Path(__file__).resolve().parents[2]
        exe_locs = [project_root / ("build/Project2.exe" if sys.platform == "win32" else "build/Project2"),
                    project_root / ("Project2.exe" if sys.platform == "win32" else "Project2")
                    ]
        
        exe = next((exec for exec in exe_locs if exec.exists()), exe_locs[0])
        csv_path = project_root / "data" / "merged_data_100k.csv"

        args = [
            str(csv_path),
            str(state),
            str(control),
            str(tuition),
            str(acceptance_rate),
            str(sat_score),
            str(school_size),
            str(weight_tuition),
            str(weight_acceptance),
            str(weight_sat),
            str(weight_size),
            str(algorithm_choice),
        ]

        result: Optional[subprocess.CompletedProcess[str]] = None

        print("DEBUG")
        print(f"Running command: {[str(exe)] + args}")
        try:
            result = subprocess.run(
                [str(exe)] + args,
                text=True,
                capture_output=True,
                check=True,
                cwd=project_root,
            )

        except subprocess.CalledProcessError as e:
            stderr = e.stderr.strip() or "Unknown error."
            messagebox.showerror("Error", f"An error occurred while running the matcher:\n{stderr}")
            return
        except FileNotFoundError:
            messagebox.showerror("Error", "Executable not found.")
            return
        except Exception as e:
            messagebox.showerror("Matcher Error", e)
            return

        output = (result.stdout or "").strip() or "No output from matcher."
        
        parsed_results = parse_matches(output)
        self.on_results(parsed_results)
        

        
    

class ResultsFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_back, results: List[Tuple[str, float]]) -> None:
        super().__init__(master, padding=24)
        self.configure(style="Card.TFrame")
        self.results = results
        self.on_back = on_back

        title = tk.Label(self, text="Match Results", font=("Courier New", 28, "bold"), fg="#1d2c79", bg="white")
        title.grid(row=0, column=0, sticky="w", pady=(0, 16))

        self.tree = ttk.Treeview(self, columns=("college", "score"), show="headings", height=14)
        self.tree.heading("college", text="College")
        self.tree.heading("score", text="Score")
        self.tree.column("college", width=540, anchor="w")
        self.tree.column("score", width=120, anchor="center")

        scroll = ttk.Scrollbar(self, orient="vertical", command=self.tree.yview)
        self.tree.configure(yscrollcommand=scroll.set)
        self.tree.grid(row=1, column=0, sticky="nsew")
        scroll.grid(row=1, column=1, sticky="ns")

        btns = ttk.Frame(self, style="Card.TFrame")
        btns.grid(row=2, column=0, columnspan=2, pady=(12, 0), sticky="e")
        ttk.Button(btns, text="Back", command=self.on_back, style="Accent.TButton").grid(row=0, column=0, padx=6)
        ttk.Button(btns, text="Exit", command=self.quit_app, style="Accent.TButton").grid(row=0, column=2, padx=6)

        self.columnconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)

        self.load_rows()

    def load_rows(self) -> None:
        rows = sorted(self.results, key=lambda x: x[1], reverse=True)
        for name, score in rows:
            score_disp = f"{score*100:.1f}%" if score else ""
            self.tree.insert("", "end", values=(name, score_disp))

    def quit_app(self) -> None:
        self.winfo_toplevel().destroy()




class CollegeMatcherApp(tk.Tk):
    # Initialize the main application window
    def __init__(self) -> None:
        super().__init__()
        self.title("College Matcher")
        self.configure(bg="#ecf0ff")
        self.geometry("960x720")

        style = ttk.Style(self)
        style.configure("Card.TFrame", background="white")
        style.configure("Accent.TButton", font=("Courier New", 16, "bold"), padding=10)
        style.configure("Horizontal.TScale", background = "white")
        style.configure("TRadiobutton", background = "white")

        self.card = ttk.Frame(self, style="Card.TFrame", padding=24)
        self.card.place(relx=0.5, rely=0.5, anchor="center")

        self.current_frame: Optional[ttk.Frame] = None
        self.form_data: dict = {}

        self.show_welcome()

    def show_welcome(self) -> None:
        self.swap_frame(WelcomeFrame(self.card, self.show_form, self.destroy))

    def show_form(self) -> None:
        self.swap_frame(FormFrame(self.card, self.show_welcome, self.show_weights))

    def show_weights(self, form_data: dict) -> None:
        self.form_data = form_data
        self.swap_frame(WeightsFrame(self.card, self.show_form, self.show_results, form_data))

    def show_results(self, results):
        on_back_weights = lambda: self.show_weights(self.form_data)
        self.swap_frame(ResultsFrame(self.card, on_back_weights, results))

    def swap_frame(self, frame: ttk.Frame) -> None:
        if self.current_frame is not None:
            self.current_frame.destroy()
        self.current_frame = frame
        self.current_frame.grid(row=0, column=0, sticky="nsew")

        self.card.columnconfigure(0, weight=1)
        self.card.rowconfigure(0, weight=1)


def main() -> None:
    app = CollegeMatcherApp()
    app.mainloop()


if __name__ == "__main__":
    main()