import { useState, useEffect } from "react";
import { motion } from "framer-motion";

function getRandomChar() {
  const chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  return chars.charAt(Math.floor(Math.random() * chars.length));
}

function ScrambledHeading({ text, delay = 100 }) {
  const [display, setDisplay] = useState(Array(text.length).fill(""));

  useEffect(() => {
    let index = 0;
    let cycles = 0;

    const interval = setInterval(() => {
      setDisplay((prev) => {
        const newDisplay = [...prev];

        for (let i = 0; i < text.length; i++) {
          if (i < index) {
            newDisplay[i] = text[i];
          } else {
            newDisplay[i] = getRandomChar();
          }
        }
        return newDisplay;
      });

      cycles++;

      if (cycles % 3 === 0) {
        index++;
      }

      if (index > text.length) clearInterval(interval);
    }, delay);

    return () => clearInterval(interval);
  }, [text, delay]);

  return (
    <h1 className="text-6xl font-extrabold text-blue-400 drop-shadow-lg select-none">
      {display.join("")}
    </h1>
  );
}

export default function LandingPage({ onStart }) {
  return (
    <div className="h-screen bg-gradient-to-br from-gray-900 via-gray-800 to-black flex flex-col items-center justify-center text-center px-6 text-gray-200 relative">
      
      <motion.div
        initial={{ opacity: 0, y: -20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.8 }}
      >
        <ScrambledHeading text="Mini C Compiler" delay={80} />
      </motion.div>

      <motion.p
        className="text-lg mt-6 max-w-xl text-gray-400"
        initial={{ opacity: 0, y: 10 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ delay: 1, duration: 0.7 }}
      >
        Analyze, compile and visualize C programs directly in your browser with a simple UI.
      </motion.p>

      <motion.button
        whileHover={{ scale: 1.05 }}
        whileTap={{ scale: 0.95 }}
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ delay: 1.5, duration: 0.5 }}
        onClick={onStart}
        className="mt-10 px-8 py-3 bg-blue-600 hover:bg-blue-700 text-white text-lg font-semibold rounded-full shadow-lg"
      >
        Start Compiling →
      </motion.button>

      <motion.div
        className="absolute bottom-0 h-32 w-full bg-gradient-to-t from-blue-900 to-transparent pointer-events-none"
        initial={{ opacity: 0 }}
        animate={{ opacity: 1 }}
        transition={{ delay: 1.8, duration: 1 }}
      />

      {/* Footer */}
      <footer className="w-full h-16 bg-gray-900 text-gray-400 text-center text-sm flex items-center justify-center absolute bottom-0 px-6">
  Made By :- Yashika, Anshika, Anshika Sharma, Anuj
  <a
    href="#"
    className="underline hover:text-blue-400 ml-2"
    target="_blank"
    rel="noopener noreferrer"
  >
    GitHub
  </a>
</footer>

    </div>
  );
}
