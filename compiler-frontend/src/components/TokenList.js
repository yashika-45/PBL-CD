import { motion } from 'framer-motion';
import { useEffect, useState, useRef } from 'react';

export default function TokenList({ output, type }) {
  const [visibleTokens, setVisibleTokens] = useState([]);
  const intervalRef = useRef(null);

  useEffect(() => {
    if (type !== 'lexical' || !Array.isArray(output)) return;

    let index = 0;
    setVisibleTokens([]);
    clearInterval(intervalRef.current);

    intervalRef.current = setInterval(() => {
      if (index >= output.length) {
        clearInterval(intervalRef.current);
        return;
      }

      const nextToken = output[index];
      if (nextToken && nextToken.lexeme && nextToken.code && nextToken.tokenName) {
        setVisibleTokens((prev) => [...prev, nextToken]);
      }

      index++;
    }, 200);

    return () => clearInterval(intervalRef.current);
  }, [output, type]);

  if (type !== 'lexical' || !Array.isArray(output)) return null;

  return (
    <div className="bg-gray-900 text-white p-4 shadow rounded max-h-96 overflow-y-auto">
      <h2 className="text-xl font-semibold text-blue-300 mb-2">Tokens</h2>
      {visibleTokens.map((token, index) => (
        token && (
          <motion.div
            key={index}
            className="bg-gray-800 p-2 mb-2 rounded font-mono text-sm shadow-md border border-gray-700"
            initial={{ opacity: 0, x: -10 }}
            animate={{ opacity: 1, x: 0 }}
            transition={{ duration: 0.3 }}
          >
            <span className="text-green-400">Lexeme:</span> {token.lexeme} &nbsp;|&nbsp;
            <span className="text-yellow-400">Code:</span> {token.code} &nbsp;|&nbsp;
            <span className="text-pink-400">Token:</span> {token.tokenName}
          </motion.div>
        )
      ))}
    </div>
  );
}
