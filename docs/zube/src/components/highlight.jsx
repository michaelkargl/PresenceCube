import React from "react"
import { MDXProvider } from '@mdx-js/react'

const Highlight = ({children, color}) => {
  console.log('Highlighting %s in %s', children, color);
  return (
    <span style={{
      backgroundColor: color,
      borderRadius: '3px',
      color: '#fff',
      padding: '0.2rem'
    }}>{children}</span>
    );
};

// make the custom component available to the MDX engine
export default ({children, color}) => (
  <MDXProvider components={{Highlight}}>
      <Highlight color={color}>{children}</Highlight>
  </MDXProvider>
);