require 'spec_helper'

describe Spyglass::Image do
  let(:lena) { Spyglass::Image.load(fixture_path('lena.jpg')) }

  describe '.load' do
    it 'should require an argument' do
      expect { Spyglass::Image.load }.to raise_error ArgumentError
    end

    it 'should return an instance of OpenCV::Image when passing a path' do
      expect( Spyglass::Image.load(fixture_path('lena.jpg')) ).to be_a described_class
    end
  end

  describe 'accessors' do
    describe '#rows' do
      it 'should return the right number of rows for an image' do
        expect( lena.rows ).to eq(512)
      end
    end

    describe '#cols' do
      it 'should return the right number of columns for an image' do
        expect( lena.cols ).to eq(512)
      end
    end

    describe '#size' do
      it 'should be an instance of Spyglass::Size' do
        expect( lena.size ).to be_a Spyglass::Size
      end

      it 'should return the correct values' do
        expect( lena.size.width ).to eq( lena.cols )
        expect( lena.size.height ).to eq( lena.rows )
      end
    end
  end

  describe '#erode' do
    it 'should erode the image' do
      eroded = lena.erode

      expect( eroded.cols ).to eq( lena.cols )
      expect( eroded.rows ).to eq( lena.rows )
    end
  end

  describe '#crop' do
    it 'should crop the image to the correct proportions' do
      rect = Spyglass::Rect.new(0, 0, 256, 256)
      cropped = lena.crop(rect)

      expect( cropped.cols ).to eq(256)
      expect( cropped.rows ).to eq(256)
    end
  end

  describe '#crop!' do
    it 'should crop the image in place to the correct proportions' do
      rect = Spyglass::Rect.new(0, 0, 256, 256)
      lena.crop!(rect)

      expect( lena.cols ).to eq(256)
      expect( lena.rows ).to eq(256)
    end
  end

  describe '#write' do
    it 'should write the image onto disk' do
      Dir.mktmpdir do |dir|
        path = File.join(dir, 'lena.jpg')

        res = lena.write(path)
        expect( File.exists?(path) ).to be_true
        expect( res ).to be_true
      end
    end
  end

  describe '#copy!' do
    it 'should copy the image inplace' do
      im = Spyglass::Image.new

      expect( im.rows ).to eq(0)
      expect( im.cols ).to eq(0)

      im.copy!(lena)

      expect( im.rows ).to eq(512)
      expect( im.cols ).to eq(512)
    end
  end
end
