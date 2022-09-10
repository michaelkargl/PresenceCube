import React from 'react'
import { MDXProvider } from '@mdx-js/react'
import { Canvas } from './canvas.jsx'

/**
 * Draws a colorful background and overlays some text.
 * @param props no direct prop input required.
 * @example <CanvasBanner>text</CanvasBanner>
 */
 const CanvasBanner = props => {
    const [
        fontColor,
        fontMargin,
        marginTop,
        marginBottom,
        canvasWidth,
        canvasHeight,
        canvasBlurRadius,
        canvasBrightnessPercent,
        canvasSaturation,
        squareCount
    ] = [
        props.fontColor ?? "white",
        props.fontMargin ?? 35,
        props.marginTop ?? 100,
        props.marginBottom ?? 50,
        props.canvasWidth ?? "100%",
        props.canvasHeight ?? "100px",
        props.canvasBlurRadius ?? 7,
        props.canvasBrightnessPercent ?? 0.5,
        props.canvasSaturation ?? 0.3,
        props.squareCount ?? 50
    ];

    /**
     * @param {number} min
     * @param {number} max
     */
    const randomNumber = (min, max) => min + (Math.random()*10**16 % max);

    /**
     * @returns {number} a random 8-bit RGB color value
     */
    const randomColor = () => randomNumber(0, 255);
    
    /**
     * Draws a rectangle to the given canvas
     * @param {*} canvas the 2d canvas context to draw onto
     * @param {number} positionX the top-left corner X position
     * @param {number} positionY the top-left corner Y position
     * @param {number} width the width in pixels
     * @param {number} height the height in pixels
     * @param {number} red an 8-bit RGB value
     * @param {number} green an 8-bit RGB value
     * @param {number} blue an 8-bit RGB value
     */
    const drawRectangle = (canvas, positionX, positionY, width, height, red, green, blue) => {
        canvas.fillStyle = `rgb(${red}, ${green}, ${blue})`;
        canvas.fillRect(positionX, positionY, width, height);
    }

    /**
     * Draws a single square of random size and color onto the provided canvas
     * @param {*} canvasContext the 2d canvas context to draw onto
     * @param {number} canvasWidth
     * @param {number} canvasHeight
     */
    const drawRandomSquare = (canvasContext, canvasWidth, canvasHeight) => {
        const size = randomNumber(0, canvasWidth);
        const [
            positionX,
            positionY,
            red,
            green,
            blue
        ] = [
            randomNumber(0, canvasWidth),
            randomNumber(0, canvasHeight),
            randomColor(),
            randomColor(),
            randomColor()
        ];

        drawRectangle(
            canvasContext,
            positionX, positionY,
            size, size,
            red, green, blue
        );
    }    

    /**
     * Draws a number of random squares onto the given canvas
     * @param {*} canvasContext the 2d canvas context to draw onto
     * @param {number} canvasWidth the x bounardy of the canvas in pixels
     * @param {number} canvasHeight the y boundary of the canvas in pixels
     * @param {number} count the amount of squares to render
     */
    const drawRandomSquares = (canvasContext, canvasWidth, canvasHeight, count) => {
        for (let i = 0; i < count; i++ ) {
            drawRandomSquare(
                canvasContext,
                canvasWidth,
                canvasHeight);
        }
    }

    /**
     * draws the banner onto the given canvas
     * @param {*} canvasContext  the 2d canvas context to draw onto
     * @param {number} canvasWidth the x bounardy of the canvas in pixels
     * @param {number} canvasHeight the y boundary of the canvas in pixels
     */
    const draw = (canvasContext, canvasWidth, canvasHeight) => {
        drawRectangle(canvasContext, 0, 0, canvasWidth, canvasHeight, 0, 0, 0);   
        drawRandomSquares(canvasContext, canvasWidth, canvasHeight, squareCount);
    }

    return (
        <div style={{height: "100%", position: "relative"}}>
            {/* overlay child elements ontop of the canvas */}
            <div style={{
                position: "absolute",
                zIndex: 1,
                color: fontColor,
                margin: fontMargin
            }}>{props.children}</div>
            
            {/* distort the canvas image to make text easier to read */}
            <div style={{marginTop, marginBottom, filter: `
                blur(${canvasBlurRadius}px)
                brightness(${canvasBrightnessPercent})
                saturate(${canvasSaturation})`
            }}>
                <Canvas width={canvasWidth}
                        height={canvasHeight}
                        onCanvasReadyFn={draw}></Canvas>
            </div>
        </div>
    );
}

export default props => (
    <MDXProvider components={{CanvasBanner}}>
        <CanvasBanner {...props}>{props.children}</CanvasBanner>
    </MDXProvider>
)