import React from 'react';
import { motion } from 'framer-motion';
// import './VisualASTViewer.css'; // optional for CSS edges

const renderTree = (node, index = 0) => {
  return (
    <motion.div
      key={`${node.label}-${index}`}
      initial={{ opacity: 0, y: -20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.5 }}
      className="relative flex flex-col items-center"
    >
      <div className="w-16 h-16 flex items-center justify-center rounded-full bg-blue-600 text-white font-bold shadow-lg">
        {node.label}
      </div>

      {node.children && node.children.length > 0 && (
        <div className="relative flex justify-center mt-6 gap-8">
          {/* Draw edge line from parent to each child */}
          {node.children.map((child, idx) => (
            <div key={idx} className="relative">
              {/* Line from current node to child */}
              <div className="absolute top-[-40px] left-1/2 h-10 border-l border-gray-400 z-[-1]" />
              {renderTree(child, idx)}
            </div>
          ))}
        </div>
      )}
    </motion.div>
  );
};

const VisualASTViewer = ({ ast }) => {
  if (!ast) return null;

  return (
    <div className="p-4 bg-gray-50 rounded-lg overflow-auto border">
      {renderTree(ast)}
    </div>
  );
};

export default VisualASTViewer;
