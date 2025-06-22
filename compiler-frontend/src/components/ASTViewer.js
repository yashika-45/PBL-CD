import React from 'react';
import VisualASTViewer from './VisualASTViewer';

const ASTViewer = ({ type, ast }) => {
  if (type !== 'syntax' || !ast) return null;

  return (
    <div className="w-full overflow-x-auto overflow-y-auto max-h-[700px]">
  <h2 className="text-2xl font-semibold text-center mb-4">Syntax Tree</h2>
  <div className="min-w-[1000px] flex justify-center">
    <VisualASTViewer ast={ast} />
  </div>
</div>

  );
};

export default ASTViewer;
