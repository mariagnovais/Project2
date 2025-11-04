import { Link } from "react-router-dom";
import { GraduationCap } from "lucide-react";

export default function Index() {
    return (
        <main className="min-h-screen grid place-items-center bg-background">
            <div className="mx-auto w-[340px] sm:w-[420px] bg-white border-[3px] border-orange-500 rounded-sm shadow-md p-6 sm:p-8 text-center">
                <div className="flex items-center justify-center gap-2">
                    <GraduationCap className="h-7 w-7 text-blue-600" />
                    <h1 className="text-[26px] sm:text-[28px] font-bold tracking-wide text-blue-600 uppercase">
                        College
                    </h1>
                </div>
                <div className="mt-3 border-t border-dashed border-neutral-400" />

                <div className="mt-6 space-y-2">
                    <p className="text-blue-600 font-semibold">Group 47</p>
                    <p className="text-blue-700">Caroline • Maria • Theron</p>
                </div>

                <Link
                    to="/match"
                    className="mt-8 inline-block rounded-sm bg-neutral-100 px-4 py-2 text-blue-700 font-semibold tracking-wider"
                >
                    [CONTINUE]
                </Link>
            </div>
        </main>
    );
}

export default function Match() {
    return (
        <main className="min-h-screen grid place-items-center bg-background">
            <div className="w-full max-w-[720px] px-4">
                <p className="mb-2 text-sm font-semibold text-purple-600">◆ START MENU</p>
                <div className="bg-white border-[3px] border-orange-500 rounded-sm shadow-md p-8 text-center">
                    <div className="flex items-center justify-center gap-3">
                        <GraduationCap className="h-7 w-7 text-blue-600" />
                        <h1 className="text-[28px] font-bold tracking-wide text-blue-600 uppercase">
                            College Matcher
                        </h1>
                    </div>
                    <div className="mt-3 border-t border-dashed border-neutral-400" />

                    <div className="mt-8 space-y-4">
                        <Link
                            to="/quiz"
                            className="mx-auto block w-max rounded-sm bg-neutral-100 px-4 py-2 text-blue-700 font-semibold tracking-wider"
                        >
                            [START MATCHING]
                        </Link>
                        <Link
                            to="/settings"
                            className="mx-auto block w-max rounded-sm bg-neutral-100 px-4 py-2 text-blue-700 font-semibold tracking-wider"
                        >
                            [SETTINGS]
                        </Link>
                        <Link
                            to="/"
                            className="mx-auto block w-max rounded-sm bg-neutral-100 px-4 py-2 text-blue-700 font-semibold tracking-wider"
                        >
                            [EXIT]
                        </Link>
                    </div>
                </div>
            </div>
        </main>
    );
}