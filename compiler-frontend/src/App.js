import { useState } from 'react';
import CodeInput from './components/CodeInput';
import TokenList from './components/TokenList';
import SymbolTable from './components/SymbolTable';
import Loader from './components/Loader';
import LandingPage from './components/LandingPage';
import ASTViewer from './components/ASTViewer';

function App() {
  const [started, setStarted] = useState(false);
  const [lexicalOutput, setLexicalOutput] = useState([]);
  const [ast, setAst] = useState(null);
  const [lexicalLoading, setLexicalLoading] = useState(false);
  const [syntaxLoading, setSyntaxLoading] = useState(false);

  const runAnalysis = async (analysisType, code) => {
    if (analysisType === 'lexical') {
      setLexicalLoading(true);
    } else if (analysisType === 'syntax') {
      setSyntaxLoading(true);
      setAst(null);
    }

    try {
      const response = await fetch('http://127.0.0.1:5000/analyze', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: new URLSearchParams({ code, type: analysisType }),
      });

      const data = await response.json();

      if (analysisType === 'lexical') {
        setLexicalOutput(data.output || []);
      } else if (analysisType === 'syntax') {
        if (data.ast) {
          try {
            setAst(JSON.parse(data.ast));
          } catch (err) {
            console.error("Failed to parse AST JSON", err);
          }
        }
      }

    } catch (error) {
      const errorMsg = `Request failed: ${error.message}`;
      if (analysisType === 'lexical') {
        setLexicalOutput([{ lexeme: '', code: '', tokenName: errorMsg }]);
      }
    }

    if (analysisType === 'lexical') {
      setLexicalLoading(false);
    } else if (analysisType === 'syntax') {
      setSyntaxLoading(false);
    }
  };

  if (!started) {
    return <LandingPage onStart={() => setStarted(true)} />;
  }

  return (
    <div className="min-h-screen p-6 bg-gray-900 text-white transition-all duration-300">
      <h1 className="text-4xl font-bold text-center text-blue-400 mb-6">Mini C Compiler</h1>

      <div className="flex flex-col lg:flex-row gap-6">
        {/* Left: Code Input */}
        <div className="w-full lg:w-1/2 bg-gray-800 p-4 rounded-xl shadow-lg">
          <CodeInput onAnalyze={runAnalysis} />
        </div>

        {/* Right: Lexical Output + Symbol Table */}
        <div className="w-full lg:w-1/2 space-y-4">
          <div className="bg-gray-800 p-4 rounded-xl shadow-lg h-[340px] overflow-y-auto">
            {lexicalLoading ? <Loader /> : <TokenList output={lexicalOutput} type="lexical" />}
          </div>
          {Array.isArray(lexicalOutput) && lexicalOutput.length > 0 && (
            <div className="bg-gray-800 p-4 rounded-xl shadow-lg h-[200px] overflow-y-auto">
              <SymbolTable tokens={lexicalOutput} />
            </div>
          )}
        </div>
      </div>

      {/* Bottom: Syntax Tree */}
      {syntaxLoading && (
        <div className="mt-8 text-center text-sm text-gray-400">Generating syntax tree...</div>
      )}
      {ast && !syntaxLoading && (
        <div className="mt-8 bg-gray-800 p-4 rounded-xl shadow-lg overflow-auto">
          <h2 className="text-xl text-center mb-4 text-blue-300">Syntax Tree</h2>
          <ASTViewer type="syntax" ast={ast} />
        </div>
      )}
    </div>
  );
}

export default App;
