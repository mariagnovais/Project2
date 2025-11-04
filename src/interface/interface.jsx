// from our figma design

export default function IntroSlide() {
    return (
        <div className="bg-neutral-100 relative size-full" data-name="INTRO SLIDE">
            <div className="relative size-full">
                <p className="absolute font-['Inter:Semi_Bold',sans-serif] font-semibold h-[116px] leading-[normal] left-[202px] not-italic right-[166px] text-[#2e5dc3] text-[96px] top-[84px]">🎓 COLLEGE MATCHER</p>
                <div className="absolute bg-[#d9d9d9] bottom-[79px] h-[86px] left-[calc(50%-0.5px)] translate-x-[-50%] w-[427px]" />
                <p className="absolute bottom-[160px] font-['Inter:Semi_Bold',sans-serif] font-semibold h-[75px] leading-[normal] left-[calc(50%-195px)] not-italic text-[#2e5dc3] text-[64px] translate-y-[100%] w-[390px]">[CONTINUE]</p>
                <div className="absolute font-['Inter:Semi_Bold',sans-serif] font-semibold h-[197px] leading-[normal] left-[calc(50%-408px)] not-italic text-[#2e5dc3] text-[64px] top-[calc(50%-99px)] w-[815px]">
                    <p className="mb-0 whitespace-pre-wrap">{`                 Group 47`}</p>
                    <p>Caroline • Maria • Theron</p>
                </div>
                <p className="absolute font-['Inter:Semi_Bold',sans-serif] font-semibold h-[102px] leading-[normal] left-[194px] not-italic right-[158px] text-[64px] text-black top-[177px]">------------------------------------</p>
            </div>
            <div aria-hidden="true" className="absolute border-[#ff6a00] border-[15px] border-solid inset-0 pointer-events-none" />
        </div>
    );
}