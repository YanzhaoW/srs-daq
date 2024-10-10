import styles from './Headline.module.css'

const Headline = () => {

    return (
        <div className={styles.headline}>
            <span className={styles.icon_text}>SRS Web Application</span>
            <div className={styles.start_div}>
                <button className={styles.start_button}>Start</button><br />
                <p className={styles.start_button_caption}>Press to start</p>
            </div>
            <div className={styles.log_window}>
                {`
                    log infomation ......
                        log infomation ......
                        log infomation ......
                        log infomation ......
                        log infomation ......
                        log infomation ......
                        `}
            </div>
            <div className={styles.right_div}>
                <div className={styles.tabs_div}>
                    <button className={styles.button}>Control</button>
                    <button className={styles.button}>Visualization</button>
                    <button className={styles.button}>Help</button>
                </div>
            </div>
        </div>
    );

}

export default Headline
