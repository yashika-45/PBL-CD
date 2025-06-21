// import { useState } from 'react';
// import CodeInput from './components/CodeInput';
// import TokenList from './components/TokenList';
// import SymbolTable from './components/SymbolTable';
// import Loader from './components/Loader';

// function App() {
//   const [output, setOutput] = useState([]);
//   const [type, setType] = useState('');
//   const [loading, setLoading] = useState(false);

//   const runAnalysis = async (analysisType, code) => {
//     setType(analysisType);
//     setLoading(true);
//     try {
//       const response = await fetch('http://127.0.0.1:5000/analyze', {
//         method: 'POST',
//         headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
//         body: new URLSearchParams({ code, type: analysisType }),
//       });

//       const data = await response.json();
//       setOutput(data.output || data.error || []);
//     } catch (error) {
//       setOutput(`Request failed: ${error.message}`);
//     }
//     setLoading(false);
//   };

//   return (
//     <div className="max-w-5xl mx-auto p-6 space-y-6">
//       <h1 className="text-4xl font-bold text-center text-blue-800">Mini C Compiler</h1>
//       <CodeInput onAnalyze={runAnalysis} />
//       {loading ? <Loader /> : <TokenList output={output} type={type} />}
//       {type === 'lexical' && Array.isArray(output) && <SymbolTable tokens={output} />}
//     </div>
//   );
// }

// export default App;


// import { useState } from 'react';
// import CodeInput from './components/CodeInput';
// import TokenList from './components/TokenList';
// import SymbolTable from './components/SymbolTable';
// import Loader from './components/Loader';
// import LandingPage from './components/LandingPage';

// function App() {
//   const [started, setStarted] = useState(false);
//   const [output, setOutput] = useState([]);
//   const [type, setType] = useState('');
//   const [loading, setLoading] = useState(false);

//   const runAnalysis = async (analysisType, code) => {
//     setType(analysisType);
//     setLoading(true);
//     try {
//       const response = await fetch('http://127.0.0.1:5000/analyze', {
//         method: 'POST',
//         headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
//         body: new URLSearchParams({ code, type: analysisType }),
//       });

//       const data = await response.json();
//       setOutput(data.output || data.error || []);
//     } catch (error) {
//       setOutput(`Request failed: ${error.message}`);
//     }
//     setLoading(false);
//   };

//   if (!started) {
//     return <LandingPage onStart={() => setStarted(true)} />;
//   }

//   return (
//     <div className="max-w-5xl mx-auto p-6 space-y-6">
//       <h1 className="text-4xl font-bold text-center text-blue-800">Mini C Compiler</h1>
//       <CodeInput onAnalyze={runAnalysis} />
//       {loading ? <Loader /> : <TokenList output={output} type={type} />}
//       {type === 'lexical' && Array.isArray(output) && <SymbolTable tokens={output} />}
//     </div>
//   );
// }

// export default App;


import { useState } from 'react';
import CodeInput from './components/CodeInput';
import TokenList from './components/TokenList';
import SymbolTable from './components/SymbolTable';
import Loader from './components/Loader';
import LandingPage from './components/LandingPage';
import ASTViewer from './components/ASTViewer'; // 🔹 NEW COMPONENT

function App() {
  const [started, setStarted] = useState(false);
  const [output, setOutput] = useState([]);
  const [type, setType] = useState('');
  const [loading, setLoading] = useState(false);
  const [ast, setAst] = useState(null); // 🔹 NEW STATE

  const runAnalysis = async (analysisType, code) => {
    setType(analysisType);
    setLoading(true);
    setAst(null); // Reset AST on new request
    try {
      const response = await fetch('http://127.0.0.1:5000/analyze', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: new URLSearchParams({ code, type: analysisType }),
      });

      const data = await response.json();
      setOutput(data.output || data.error || []);
      console.log("Received output:", data.output || data.error);
      if (analysisType === 'syntax' && data.ast) {
        try {
          setAst(JSON.parse(data.ast));
        } catch (err) {
          console.error("Failed to parse AST JSON", err);
        }
      }
    } catch (error) {
      setOutput(`Request failed: ${error.message}`);
    }
    setLoading(false);
  };

  if (!started) {
    return <LandingPage onStart={() => setStarted(true)} />;
  }

  return (
    <div className="max-w-5xl mx-auto p-6 space-y-6">
      <h1 className="text-4xl font-bold text-center text-blue-800">Mini C Compiler</h1>
      <CodeInput onAnalyze={runAnalysis} />
      {loading ? (
        <Loader />
      ) : (
        <>
          <TokenList output={output} type={type} />
          {type === 'lexical' && Array.isArray(output) && <SymbolTable tokens={output} />}
         {type === 'syntax' && ast && <ASTViewer type={type} ast={ast} />}
        </>
      )}
    </div>
  );
}

export default App;
