import { useEffect, useState } from "react";

const characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

function getRandomChar() {
  return characters[Math.floor(Math.random() * characters.length)];
}

export default function ScrambledHeading({ text, delay = 120 }) {
  const [display, setDisplay] = useState(Array(text.length).fill(""));

  useEffect(() => {
    let index = 0;

    const interval = setInterval(() => {
      setDisplay((prev) => {
        const newDisplay = [...prev];

        for (let i = 0; i < text.length; i++) {
          if (i < index) {
            newDisplay[i] = text[i]; // lock correct letter
          } else {
            newDisplay[i] = getRandomChar(); // scramble upcoming letters
          }
        }

        return newDisplay;
      });

      index++;

      if (index > text.length) {
        clearInterval(interval);
      }
    }, delay);

    return () => clearInterval(interval);
  }, [text, delay]);

  return (
    <h1 className="text-6xl font-extrabold text-blue-800 drop-shadow-md font-mono tracking-wide">
      {display.join("")}
    </h1>
  );
}
