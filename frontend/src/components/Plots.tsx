import styles from './Plots.module.css'
import bb, { line, areaSpline } from "billboard.js"
import BillboardJS, { IChart } from "@billboard.js/react";
import "billboard.js/dist/billboard.css";
import { useRef, useEffect } from "react";

const PLOT_WIDTH_PERCENT = 0.8
const PLOT_HEIGHT_PERCENT = 0.4

const setAjustableSize = (lineChart: React.RefObject<IChart>, plot_ref: React.RefObject<HTMLHeadingElement>) => {
    if (plot_ref.current) {
        const chart = lineChart.current?.instance;
        if (chart) {
            chart.resize({ width: PLOT_WIDTH_PERCENT * plot_ref.current.offsetWidth, height: PLOT_HEIGHT_PERCENT * plot_ref.current.offsetHeight });
        }
    }
}

interface CommonPlotSetup<Type> {
    x_label: string;
    y_label: string;
    title: string;
    type?: Type;
}

const CreatePlotOptions = <Type,>(setup: CommonPlotSetup<Type>) => {
    return {
        title: { text: setup.title },
        data: {
            columns: [["data", 30, 200, 100, 400, 150, 250],],
            type: setup.type
        },
        axis: {
            x: {
                label: {
                    text: setup.x_label,
                    position: "outer-center"
                },
            },
            y: {

                label: {
                    text: setup.y_label,
                    position: "outer-middle"
                },
            }
        },
        legend: { show: false },
    };
};

const Plots = () => {

    const lineChart1 = useRef<IChart>(null);
    const lineChart2 = useRef<IChart>(null);
    const frame_ref = useRef<HTMLHeadingElement>(null);

    const plotInit = (chart: React.RefObject<IChart>) => {
        setAjustableSize(chart, frame_ref);
        const handleResize = () => {
            setAjustableSize(chart, frame_ref);
        };
        window.addEventListener('resize', handleResize);
        return () => {
            window.removeEventListener('resize', handleResize);
        };
    };

    useEffect(() => plotInit(lineChart1), []);
    useEffect(() => plotInit(lineChart2), []);

    const option1 = CreatePlotOptions({ x_label: "Time (seconds)", y_label: "Data received (Bytes)", title: "Data Bytes Receive Rate", type: areaSpline() });
    const option2 = CreatePlotOptions({ x_label: "Time (seconds)", y_label: "Hits analysed", title: "Analysed hits Rate", type: line() });

    return (
        <div className={styles.plot} ref={frame_ref}>
            <div className={styles.plot_top}>
                <BillboardJS
                    bb={bb}
                    options={option1}
                    ref={lineChart1}
                    className={"bb"}
                />
            </div>
            <div className={styles.plot_top}>
                <BillboardJS
                    bb={bb}
                    options={option2}
                    ref={lineChart2}
                    className={"bb"}
                />
            </div>
        </div >
    );

};

export default Plots;
