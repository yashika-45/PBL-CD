import { useState } from 'react';

export default function CodeInput({ onAnalyze }) {
  const [code, setCode] = useState('');
  const [fileMode, setFileMode] = useState(false);

  const handleFileUpload = (e) => {
    const reader = new FileReader();
    reader.onload = () => setCode(reader.result);
    reader.readAsText(e.target.files[0]);
  };

  return (
    <div className="bg-gray-900 text-white shadow rounded p-4">
      <div className="flex mb-4 space-x-4">
        <button
          onClick={() => setFileMode(false)}
          className={`px-4 py-2 rounded font-semibold ${
            !fileMode ? 'bg-blue-600 text-white' : 'bg-gray-700 text-gray-300'
          }`}
        >
          Write Code
        </button>
        <button
          onClick={() => setFileMode(true)}
          className={`px-4 py-2 rounded font-semibold ${
            fileMode ? 'bg-blue-600 text-white' : 'bg-gray-700 text-gray-300'
          }`}
        >
          Upload File
        </button>
      </div>

      {fileMode ? (
        <input
          type="file"
          onChange={handleFileUpload}
          className="block w-full text-sm text-gray-300 bg-gray-800 border border-gray-600 p-2 rounded"
        />
      ) : (
        <textarea
          className="w-full h-64 p-3 bg-gray-800 text-white border border-gray-600 rounded font-mono resize-none focus:outline-none focus:ring-2 focus:ring-blue-500"
          placeholder="Enter C code here..."
          value={code}
          onChange={(e) => setCode(e.target.value)}
        />
      )}

      <div className="flex justify-center mt-4 space-x-4">
        <button
          onClick={() => onAnalyze('lexical', code)}
          className="bg-blue-700 hover:bg-blue-800 text-white px-6 py-2 rounded shadow"
        >
          Run Lexical Analysis
        </button>
        <button
          onClick={() => onAnalyze('syntax', code)}
          className="bg-green-700 hover:bg-green-800 text-white px-6 py-2 rounded shadow"
        >
          Run Syntax Analysis
        </button>
      </div>
    </div>
  );
}
