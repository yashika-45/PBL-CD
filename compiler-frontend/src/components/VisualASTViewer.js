// import React, { useMemo } from 'react';
// import { motion } from 'framer-motion';

// // Beautified Node component
// const Node = ({ label, delay }) => (
//   <motion.div
//     initial={{ opacity: 0, scale: 0.8 }}
//     animate={{ opacity: 1, scale: 1 }}
//     transition={{ duration: 0.4, delay }}
//     className="w-24 h-24 flex items-center justify-center bg-gradient-to-br from-blue-500 to-blue-700 text-white font-semibold text-sm rounded-full shadow-xl border border-white"
//   >
//     {label}
//   </motion.div>
// );

// // Curved edge using Bezier path
// const Edge = ({ fromX, fromY, toX, toY }) => {
//   const deltaX = (toX - fromX) / 2;
//   const path = `M${fromX},${fromY} C${fromX + deltaX},${fromY} ${toX - deltaX},${toY} ${toX},${toY}`;
//   return <path d={path} stroke="#888" strokeWidth="2" fill="none" />;
// };

// // Recursive renderer
// const renderTree = (node, depth = 0, index = 0, siblingCount = 1, path = '0') => {
//   const spacing = 200; // adjust spacing between siblings
//   const offsetX = spacing * (index - (siblingCount - 1) / 2);
//   const offsetY = depth * 160 + 100;

//   const delay = depth * 0.2 + index * 0.05;

//   const currentNode = (
//     <motion.foreignObject
//       key={`node-${path}`}
//       x={offsetX - 60}
//       y={offsetY - 60}
//       width="120"
//       height="120"
//       initial={{ opacity: 0 }}
//       animate={{ opacity: 1 }}
//       transition={{ delay }}
//     >
//       <Node label={node.label} delay={delay} />
//     </motion.foreignObject>
//   );

//   let children = [];

//   if (node.children && node.children.length > 0) {
//     node.children.forEach((child, i) => {
//       const childOffsetX = spacing * (i - (node.children.length - 1) / 2);
//       const childPath = `${path}-${i}`;

//       children.push(
//         <Edge
//           key={`edge-${path}-${i}`}
//           fromX={offsetX}
//           fromY={offsetY}
//           toX={childOffsetX}
//           toY={(depth + 1) * 160 + 100}
//         />,
//         ...renderTree(child, depth + 1, i, node.children.length, childPath)
//       );
//     });
//   }

//   return [currentNode, ...children];
// };

// // Main visualizer
// const VisualASTViewer = ({ ast }) => {
//   const treeElements = useMemo(() => (ast ? renderTree(ast) : []), [ast]);

//   return (
//     <div className="w-full overflow-auto">
//       <svg
//         width="3000"
//         height="2000"
//         viewBox="-1500 0 3000 2000"
//         className="mx-auto"
//       >
//         {treeElements}
//       </svg>
//     </div>
//   );
// };

// export default VisualASTViewer;


import React, { useMemo } from 'react';
import { motion } from 'framer-motion';

const NODE_WIDTH = 120;
const NODE_HEIGHT = 120;
const HORIZONTAL_SPACING = 160;
const VERTICAL_SPACING = 160;
const Node = ({ label, x, y, delay }) => (
  <motion.foreignObject
    x={x - NODE_WIDTH / 2}
    y={y - NODE_HEIGHT / 2}
    width={NODE_WIDTH}
    height={NODE_HEIGHT}
    initial={{ opacity: 0, scale: 0.8 }}
    animate={{ opacity: 1, scale: 1 }}
    transition={{ duration: 0.4, delay }}
  >
    <div className="w-full h-full flex items-center justify-center bg-gradient-to-br from-blue-500 to-blue-700 text-white font-bold text-lg rounded-full shadow-xl border border-white">
      {label}
    </div>
  </motion.foreignObject>
);


const Edge = ({ fromX, fromY, toX, toY }) => {
  const dx = (toX - fromX) / 2;
  const path = `M${fromX},${fromY} C${fromX + dx},${fromY} ${toX - dx},${toY} ${toX},${toY}`;
  return <path d={path} stroke="#444" strokeWidth="2.5" fill="none" />;
};

// Recursive layout function with tidy tree algorithm
const layoutTree = (node, depth = 0, nextX = { value: 0 }) => {
  if (!node) return [];

  let childrenLayouts = [];
  let childXs = [];

  if (node.children && node.children.length > 0) {
    node.children.forEach((child) => {
      const childLayout = layoutTree(child, depth + 1, nextX);
      childrenLayouts.push(...childLayout);
      const last = childLayout[childLayout.length - 1];
      if (last) childXs.push(last.x);
    });
  }

  let x;
  if (childXs.length > 0) {
    x = (Math.min(...childXs) + Math.max(...childXs)) / 2;
  } else {
    x = nextX.value * HORIZONTAL_SPACING;
    nextX.value++;
  }

  const y = depth * VERTICAL_SPACING + 100;

  const current = {
    label: node.label,
    x,
    y,
    children: node.children,
    id: Math.random().toString(36).substr(2, 9),
    delay: depth * 0.2 + nextX.value * 0.02,
  };

  return [...childrenLayouts, current];
};

const VisualASTViewer = ({ ast }) => {
  const layout = useMemo(() => (ast ? layoutTree(ast) : []), [ast]);

  return (
    <div className="w-full overflow-auto bg-gray-900 p-4 rounded-md shadow-inner min-h-[800px]">
      <svg
        width="3000"
        height="2000"
        viewBox="-1500 0 3000 2000"
        className="mx-auto"
      >
        {/* Edges */}
        {layout.flatMap((node) =>
          (node.children || []).map((child) => {
            const childNode = layout.find(
              (n) => n.label === child.label && n.y > node.y
            );
            if (!childNode) return null;
            return (
              <Edge
                key={`edge-${node.id}-${childNode.id}`}
                fromX={node.x}
                fromY={node.y}
                toX={childNode.x}
                toY={childNode.y}
              />
            );
          })
        )}

        {/* Nodes */}
        {layout.map((node) => (
          <Node
            key={`node-${node.id}`}
            label={node.label}
            x={node.x}
            y={node.y}
            delay={node.delay}
          />
        ))}
      </svg>
    </div>
  );
};

export default VisualASTViewer;
