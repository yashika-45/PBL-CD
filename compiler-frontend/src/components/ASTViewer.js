// src/components/ASTViewer.js
import React from 'react';
import VisualASTViewer from './VisualASTViewer';

const ASTViewer = ({ type, ast }) => {
  if (type !== 'syntax' || !ast) return null;
  return (
    <div>
      <h2 className="text-2xl font-semibold text-center mb-4">Syntax Tree</h2>
      <VisualASTViewer ast={ast} />
    </div>
    
  );
};

export default ASTViewer;
