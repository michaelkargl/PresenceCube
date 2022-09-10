import React, { useRef, useEffect } from 'react'

/**
 * Provides a resizable canvas element to draw onto
 * @param {number} props: a css width string that defines the final size of the canvas
 * @param {number} height: a css height string that defines the final size of the canvas
 * @param {(CanvasContext, number, number) => void} onCanvasReadyFn a function that gets 
 *         called once the canvas is ready for inputs.
 * @returns 
 */
export const Canvas = ({width, height, onCanvasReadyFn}) => {
    const canvasRef = useRef(null);

    /**
     * Resizes an HTML element by modifying its style properties
     * @param {HTMLElement} element 
     * @param {number | string} width
     * @param {number | string} height 
     * @returns an object containing the new width and height of the elements recalculated bounding box
     * @example resizeElement(document, 100, 200)
     * @example resizeElement(document, "100px", "50%")
     */
    const resizeElement = (element, width, height) => {
        console.group("Resizing element %o to %s x %s", element, width, height);
        element.style.width = width;
        element.style.height = height;

        const elementSize = element.getBoundingClientRect();
        console.debug("New size: %ipx x %ipx", elementSize.width, elementSize.height);
        console.groupEnd();

        return elementSize;
    }

    useEffect(() => {
        console.debug("Component mounted...");
        const canvas = canvasRef.current;
        
        // now that the parents are rendered / its sizes have stabilized, we can size the canvas
        const canvasSize = resizeElement(canvas, width, height);
        onCanvasReadyFn(
            canvas.getContext('2d'),
            canvasSize.width,
            canvasSize.height);
    }, [ onCanvasReadyFn ]);

    return <canvas ref={canvasRef} width={width} height={height}></canvas>;
};