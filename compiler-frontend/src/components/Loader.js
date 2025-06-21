
import { motion } from 'framer-motion';

export default function Loader() {
  return (
    <motion.div
      className="text-center text-blue-600 font-semibold"
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      transition={{ repeat: Infinity, repeatType: "mirror", duration: 0.6 }}
    >
      Analyzing...
    </motion.div>
  );
}
