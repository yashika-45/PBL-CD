// import { motion } from 'framer-motion';
// import { useEffect, useState, useRef } from 'react';

// export default function TokenList({ output, type }) {
//   const [visibleTokens, setVisibleTokens] = useState([]);
//   const intervalRef = useRef(null); // to store interval reference safely

//   useEffect(() => {
//     if (type !== 'lexical' || !Array.isArray(output)) return;

//     let index = 0;
//     setVisibleTokens([]); // reset visible tokens

//     clearInterval(intervalRef.current); // clear any previous interval

//     intervalRef.current = setInterval(() => {
//       if (index >= output.length) {
//         clearInterval(intervalRef.current);
//         return;
//       }

//       setVisibleTokens(prev => [...prev, output[index]]);
//       index++;
//     }, 200); // speed control (ms per token)

//     return () => clearInterval(intervalRef.current); // cleanup on unmount or prop change
//   }, [output, type]);

//   if (type !== 'lexical' || !Array.isArray(output)) return null;

//   return (
//     <div className="bg-white p-4 shadow rounded max-h-96 overflow-y-auto">
//       <h2 className="text-xl font-semibold text-blue-800 mb-2">Tokens</h2>
//       {visibleTokens.map((token, index) => (
//         <motion.div
//           key={index}
//           className="bg-blue-100 p-2 mb-2 rounded font-mono text-sm shadow"
//           initial={{ opacity: 0, x: -10 }}
//           animate={{ opacity: 1, x: 0 }}
//           transition={{ duration: 0.3 }}
//         >
//           Lexeme: {token.lexeme} | Code: {token.code} | Token: {token.tokenName}
//         </motion.div>
//       ))}
//     </div>
//   );
// }



import { motion } from 'framer-motion';
import { useEffect, useState, useRef } from 'react';

export default function TokenList({ output, type }) {
  const [visibleTokens, setVisibleTokens] = useState([]);
  const intervalRef = useRef(null);

  useEffect(() => {
    if (type !== 'lexical' || !Array.isArray(output)) return;

    let index = 0;
    setVisibleTokens([]); // clear tokens
    clearInterval(intervalRef.current);

    intervalRef.current = setInterval(() => {
      if (index >= output.length) {
        clearInterval(intervalRef.current);
        return;
      }

      // Defensive check before adding
      const nextToken = output[index];
      if (nextToken && nextToken.lexeme && nextToken.code && nextToken.tokenName) {
        setVisibleTokens(prev => [...prev, nextToken]);
      }

      index++;
    }, 200); // Adjust speed as needed

    return () => clearInterval(intervalRef.current);
  }, [output, type]);

  if (type !== 'lexical' || !Array.isArray(output)) return null;

  return (
    <div className="bg-white p-4 shadow rounded max-h-96 overflow-y-auto">
      <h2 className="text-xl font-semibold text-blue-800 mb-2">Tokens</h2>
      {visibleTokens.map((token, index) => (
        token && (
          <motion.div
            key={index}
            className="bg-blue-100 p-2 mb-2 rounded font-mono text-sm shadow"
            initial={{ opacity: 0, x: -10 }}
            animate={{ opacity: 1, x: 0 }}
            transition={{ duration: 0.3 }}
          >
            Lexeme: {token.lexeme} | Code: {token.code} | Token: {token.tokenName}
          </motion.div>
        )
      ))}
    </div>
  );
}
