from typing import Optional
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import subprocess
import sys

def make_label(parent: tk.Widget, text: str, **grid_kwargs: int) -> tk.Label:
    label = tk.Label(parent, text=text, anchor="w", font=("Courier New", 16), fg="#1d2c79", bg="white")
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
TUITION_OPTIONS = ("Any", "<$20k", "$20k–$40k", "$40k–$60k", ">$60k")
ACCEPT_OPTIONS = ("Any", "<20%", "20%–50%", "50%–80%", ">80%")

class FormFrame(ttk.Frame):
    def __init__(self, master: tk.Misc, on_back) -> None:
        super().__init__(master, padding=32)
        self.configure(style="Card.TFrame")
        self.on_back = on_back

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
        ttk.Button(button_frame, text="Start Matching", command=self.run_matcher(), style="Accent.TButton").grid(row=0, column=1, padx=6)

        for column in range(2):
            self.columnconfigure(column, weight=1)

        name_entry.focus_set()

    def run_matcher(self):
        args = [
            self.name_var.get(),
            self.sat_var.get(),
            self.state_var.get(),
            self.control_var.get(),
            self.size_var.get(),
            self.tuition_var.get(),
            self.acceptance_var.get(),
        ]

        exe = r"build/Project2.exe" if sys.platform == "win32" else "./Project2"

        try:
            result = subprocess.run(
                [exe] + args,
                text=True,
                capture_output=True,
                check=True
            )

        except subprocess.CalledProcessError as e:
            messagebox.showerror("Error", f"An error occurred while running the matcher:\\n{e.stderr.strip()}")
        except FileNotFoundError:
            messagebox.showerror("Error", f"Executable not found: {exe}")

        output = result.stdout.strip() or "No output from matcher."
        print("Algorithm Output:\\n" + output)
        messagebox.showinfo("Match Results: ", output)
            
    def print_summary(self) -> None:
        summary = (
            f"Name: {self.name_var.get()}\n"
            f"SAT Score: {self.sat_var.get()}\n"
            f"State: {self.state_var.get()}\n"
            f"Control: {self.control_var.get()}\n"
            f"Campus Size: {self.size_var.get()}\n"
            f"Tuition: {self.tuition_var.get()}\n"
            f"Acceptance Rate: {self.acceptance_var.get()}"
        )
        print("Preferences captured:\n" + summary)


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

        self.card = ttk.Frame(self, style="Card.TFrame", padding=24)
        self.card.place(relx=0.5, rely=0.5, anchor="center")

        self.current_frame: Optional[ttk.Frame] = None
        self.show_welcome()

    def show_welcome(self) -> None:
        self.swap_frame(WelcomeFrame(self.card, self.show_form, self.destroy))

    def show_form(self) -> None:
        self.swap_frame(FormFrame(self.card, self.show_welcome))

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