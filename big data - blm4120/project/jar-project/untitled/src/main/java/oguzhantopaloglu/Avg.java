package oguzhantopaloglu;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Avg {

    public static class AverageScoreMapper extends Mapper<Object, Text, Text, IntWritable> {

        private Text keyText = new Text("average");
        private IntWritable score = new IntWritable();

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            String[] columns = value.toString().split(",");

            if (columns.length == 6) {
                try {
                    int scoreValue = Integer.parseInt(columns[3]);
                    score.set(scoreValue);
                    context.write(keyText, score);
                }
                catch (Exception e) {}
            }
        }
    }

    public static class AverageScoreReducer extends Reducer<Text, IntWritable, Text, IntWritable> {

        public void reduce(Text key, Iterable<IntWritable> values, Context context)
                throws IOException, InterruptedException {

            int sum = 0;
            int count = 0;

            for (IntWritable value : values) {
                sum += value.get();
                count++;
            }

            int average = sum / count;

            context.write(key, new IntWritable(average));
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Average Score Finder");

        job.setJarByClass(Avg.class);
        job.setMapperClass(AverageScoreMapper.class);
        job.setReducerClass(AverageScoreReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        /*
        long starting, ending;
        starting = System.currentTimeMillis();
        job.waitForCompletion(true);
        ending = System.currentTimeMillis();
        System.out.println("Elapsed time: " + (ending-starting)); // or show it in GUI.
        */
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
